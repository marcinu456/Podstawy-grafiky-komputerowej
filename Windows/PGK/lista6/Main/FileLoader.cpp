#include "FileLoader.hpp"

FileLoader::FileLoader()
{
    filesOpened.reserve(180);
    std::string water = "hgt_files\\water.hgt";
    if (!std::filesystem::exists(water)) {
        std::cout << "create\n";
        std::ofstream ofs(water, std::ios::binary);
        int16_t water_num = -1;
        for (int i = 0; i < 1201 * 1201; i++) {
            ofs.write(reinterpret_cast<char*>(&water_num), sizeof(water_num));
        }
        ofs.close();
    }
    FILE* water_ptr = fopen(water.c_str(), "rb");

    for (int i = 0; i < 180; i++) {
        filesOpened.emplace_back(std::vector<FILE*>());
        filesOpened.back().reserve(360);
        for (int j = 0; j < 360; j++) {
            std::string filename = "hgt_files\\";
            std::string NS = std::to_string(std::abs(i - 90));
            if (i - 90 >= 0) {
                filename.append("N")
                    .append(std::string(2 - NS.length(), '0'))
                    .append(NS);
            }
            else
            {
                filename.append("S")
                    .append(std::string(2 - NS.length(), '0'))
                    .append(NS);
            }

            std::string EW = std::to_string(std::abs(j - 180));
            if (j - 180 >= 0) {
                filename.append("E")
                    .append(std::string(3 - EW.length(), '0'))
                    .append(EW);
            }
            else
            {
                filename.append("W")
                    .append(std::string(3 - EW.length(), '0'))
                    .append(EW);
            }
            filename.append(".hgt");
            if (std::filesystem::exists(filename)) {
                this->filesOpened[i].emplace_back(fopen(filename.c_str(), "rb"));
            }
            else {
                this->filesOpened[i].emplace_back(water_ptr);
            }

        }
    }
}


FILE* FileLoader::getFilePointer(int verDeg, int horDeg) {
    return filesOpened[verDeg][horDeg];
}

void FileLoader::getHeight(int16_t& n, size_t verDeg, size_t horDeg, size_t verI, size_t horI) {
    FILE* file = getFilePointer(verDeg, horDeg);
    fseek(file, ((verI + horI) % (1201 * 1201)) * sizeof(int16_t), SEEK_SET);
    fread(&n, sizeof(int16_t), 1, file);
    uint16_t hi = (n & 0xff00); // isolate the upper byte with the AND operator

    uint16_t lo = (n & 0xff); // isolate the lower byte with the AND operator
    uint16_t y = (lo << 8); // shift the lower byte to the high position and assign it to y
    y |= (hi >> 8);         // OR in the upper half, into the low position
    n = y;
  
}



FileLoader::~FileLoader() {
    for (int i = 0; i < 180; i++) {
        for (int j = 0; j < 360; j++) {
            fclose(filesOpened[i][j]);
        }
    }
}