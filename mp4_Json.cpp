#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

// JSON verisini temsil eden bir struct
struct VideoItem {
    int id;
    std::string name;

    // VideoItem'dan JSON nesnesi oluşturan işlev
    json toJson() const {
        json j;
        j["id"] = id;
        j["name"] = name;
        return j;
    }
};

class VideoManager {
private:
    json videoJson;
    std::string jsonFileName;
    std::string folderPath; // Video dosyalarının bulunduğu klasör yolu

public:
    VideoManager(const std::string& fileName, const std::string& folder) : jsonFileName(fileName), folderPath(folder) {
        loadJson();
    }

    void scanFolder() {
        int idCounter = 1;
        videoJson.clear();

        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.path().extension() == ".mp4") {
                VideoItem item;
                item.id = idCounter++;
                item.name = entry.path().filename().string();

                // VideoItem'i JSON nesnesine dönüştürüyoruz ve JSON nesnesine ekliyoruz
                videoJson["items"].push_back(item.toJson());
            }
        }
        saveJson();
    }

    void addJson(const std::string& videoName) {
        int id = videoJson["items"].empty() ? 1 : videoJson["items"].back()["id"] + 1;
        VideoItem item;
        item.id = id;
        item.name = videoName;

        // VideoItem'i JSON nesnesine dönüştürün ve JSON nesnesini ekleyin
        videoJson["items"].push_back(item.toJson());
        saveJson();
    }
    
    int temp;

    void deleteItem(int id) {
    auto& items = videoJson["items"];
    std::string deletedVideoName; // Silinen video dosyasının adını saklamak için değişken

    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it)["id"] == id) {
            deletedVideoName = (*it)["name"];
            items.erase(it); // JSON nesnesinden video öğesini sil
            break;
        }
    }

    if (!deletedVideoName.empty()) {
        saveJson(); // JSON dosyasını güncelle
        std::string videoPath = folderPath + "\\" + deletedVideoName;
        fs::remove(videoPath); // Video dosyasını klasörden sil
    }
}



    void printJson() {
        std::cout << videoJson.dump(4) << std::endl;
    }

private:
    void loadJson() {
        std::ifstream inputFile(jsonFileName);
        if (inputFile.is_open()) {
            inputFile >> videoJson;
            inputFile.close();
        }
    }

    void saveJson() {
        std::ofstream outputFile(jsonFileName);
        if (outputFile.is_open()) {
            outputFile << videoJson.dump(4);
            outputFile.close();
        }
    }
};

int main() {
    VideoManager manager("videos.json", "C:\\Asisguard\\mp4_Json\\data");

    // Klasörü tarayarak JSON oluştur
    manager.scanFolder();

    // JSON'a video eklemek için:
    //manager.addJson("deneme.mp4");

    // JSON'dan video silmek için:
    manager.deleteItem(8); // Örneğin, ID 9'a sahip videoyu sil

    // JSON verisini görüntülemek için:
    manager.printJson();

    return 0;
}
