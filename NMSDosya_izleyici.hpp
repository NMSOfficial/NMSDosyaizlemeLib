#ifndef NMSDOSYA_IZLEYICI_HPP
#define NMSDOSYA_IZLEYICI_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include <thread>
#include <filesystem>
#include <atomic>

class NMSDosyaIzleyici {
public:
    NMSDosyaIzleyici();
    ~NMSDosyaIzleyici();
    
    void izlemeyeBasla(const std::string& dizin);
    void durdur();
    
    void dosyaOlusturma(std::function<void(const std::string&)> callback);
    void dosyaSilme(std::function<void(const std::string&)> callback);
    void dosyaDegistirme(std::function<void(const std::string&)> callback);

private:
    void izle();
    std::string izlenenDizin;
    std::atomic<bool> izlemeAktif;
    std::thread izlemeThread;
    std::unordered_map<std::string, std::filesystem::file_time_type> dosyaZamanDamgalari;
    std::function<void(const std::string&)> olusturmaCallback;
    std::function<void(const std::string&)> silmeCallback;
    std::function<void(const std::string&)> degistirmeCallback;
};

#endif // NMSDOSYA_IZLEYICI_HPP
