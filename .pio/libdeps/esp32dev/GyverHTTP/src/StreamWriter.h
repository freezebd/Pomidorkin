#pragma once
#include <Arduino.h>

#include "utils/cfg.h"

#define WRITER_PRINT_BLOCK_SIZE 512

// ==================== SENDER ====================
class StreamWriter : public Printable {
   public:
    StreamWriter() {}
    StreamWriter(Stream* stream, size_t len) : _stream(stream), _len(len) {}
    StreamWriter(const uint8_t* buf, size_t len, bool pgm = 0) : _buf(buf), _len(len), _pgm(pgm) {}
    StreamWriter(const char* buf, int16_t len = -1, bool pgm = 0) : _buf((const uint8_t*)buf), _len(len >= 0 ? len : (pgm ? strlen_P(buf) : strlen(buf))), _pgm(pgm) {}
    StreamWriter(const __FlashStringHelper* str) : _buf((const uint8_t*)str), _len(strlen_P((PGM_P)str)), _pgm(true) {}
    StreamWriter(String& s) : _buf((const uint8_t*)s.c_str()), _len(s.length()), _pgm(false) {}

    // размер данных
    size_t length() const {
        return _len;
    }

    // установить размер блока отправки
    void setBlockSize(size_t bsize) {
        _bsize = bsize;
    }

    // напечатать в принт
    size_t printTo(Print& p) const {
        if (!_len) return 0;
        if (_stream) return _printStream(p);
        else if (_buf) return _pgm ? _printPGM(p) : _print(p);
        return 0;
    }

   protected:
    Stream* _stream = nullptr;
    const uint8_t* _buf = nullptr;
    size_t _len = 0;
    bool _pgm = 0;

   private:
    size_t _bsize = 128;

    size_t _printStream(Print& p) const {
        if (!_stream->available()) return 0;
        uint8_t* buf = new uint8_t[min(_bsize, _len)];
        if (!buf) return 0;

        size_t left = _len;
        size_t printed = 0;

        while (left) {
            GHTTP_ESP_YIELD();
            size_t len = min(min(left, (size_t)_stream->available()), _bsize);
            size_t read = _stream->readBytes(buf, len);
            printed += p.write(buf, read);
            if (len != read) break;
            left -= len;
        }
        delete[] buf;
        return printed;
    }

    size_t _printPGM(Print& p) const {
#if defined(ESP32)
        return _print(p);
#else
        const uint8_t* bytes = _buf;
        uint8_t* buf = new uint8_t[min(_bsize, _len)];
        if (!buf) return 0;

        size_t left = _len;
        size_t printed = 0;

        while (left) {
            GHTTP_ESP_YIELD();
            size_t len = min(_bsize, left);
            memcpy_P(buf, bytes, len);
            printed += p.write(buf, len);
            bytes += len;
            left -= len;
        }
        delete[] buf;
        return printed;
#endif
    }
    
    size_t _print(Print& p) const {
#if defined(ESP8266)
        return p.write(_buf, _len);
#elif defined(ESP32)
        size_t left = _len;
        size_t printed = 0;
        const uint8_t* bytes = _buf;
        while (left) {
            size_t curlen = min(left, (size_t)WRITER_PRINT_BLOCK_SIZE);
            printed += p.write(bytes, curlen);
            left -= curlen;
            bytes += curlen;
        }
        return printed;
#else
        return p.write(_buf, _len);
#endif
    }
};
