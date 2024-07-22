#include "NMSDosya_izleyici.hpp"
#include <iostream>
#include <chrono>
#include <thread>

NMSDosyaIzleyici::NMSDosyaIzleyici() : izlemeAktif(false) {}

NMSDosyaIzleyici::~NMSDosyaIzleyici() {
    durdur();
}

void NMSDosyaIzleyici::izlemeyeBasla(const std::string& dizin) {
    izlenenDizin = dizin;
    izlemeAktif = true;
    izlemeThread = std::thread(&NMSDosyaIzleyici::izle, this);
}

void NMSDosyaIzleyici::durdur() {
    izlemeAktif = false;
    if (izlemeThread.joinable()) {
        izlemeThread.join();
    }
}

void NMSDosyaIzleyici::dosyaOlusturma(std::function<void(const std::string&)> callback) {
    olusturmaCallback = callback;
}

void NMSDosyaIzleyici::dosyaSilme(std::function<void(const std::string&)> callback) {
    silmeCallback = callback;
}

void NMSDosyaIzleyici::dosyaDegistirme(std::function<void(const std::string&)> callback) {
    degistirmeCallback = callback;
}

void NMSDosyaIzleyici::izle() {
    while (izlemeAktif) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        for (const auto& entry : std::filesystem::directory_iterator(izlenenDizin)) {
            auto path = entry.path().string();
            auto lastWriteTime = std::filesystem::last_write_time(entry);

            if (dosyaZamanDamgalari.find(path) == dosyaZamanDamgalari.end()) {
                dosyaZamanDamgalari[path] = lastWriteTime;
                if (olusturmaCallback) {
                    olusturmaCallback(path);
                }
            } else {
                if (dosyaZamanDamgalari[path] != lastWriteTime) {
                    dosyaZamanDamgalari[path] = lastWriteTime;
                    if (degistirmeCallback) {
                        degistirmeCallback(path);
                    }
                }
            }
        }

        for (auto it = dosyaZamanDamgalari.begin(); it != dosyaZamanDamgalari.end(); ) {
            if (!std::filesystem::exists(it->first)) {
                if (silmeCallback) {
                    silmeCallback(it->first);
                }
                it = dosyaZamanDamgalari.erase(it);
            } else {
                ++it;
            }
        }
    }
}
