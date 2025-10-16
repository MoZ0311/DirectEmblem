// CSVReader

# pragma once

# include <fstream>
# include <sstream>
# include "../core/Config.hpp"

struct CSVReader
{
    static inline std::vector<std::vector<std::string>> readCsvFile(const std::string& filePath, char delimiter = ',')
    {
        // データ格納用の二次元配列
        std::vector<std::vector<std::string>> data{};

        // ファイルの読み込み
        std::ifstream file{ filePath };
        if (!file.is_open())
        {
            // ファイルが開けなかった場合空の配列をreturn
            return data;
        }

        // 行ごとの読み取り
        std::string line{};
        while (std::getline(file, line))
        {
            // 行(row)を格納
            std::vector<std::string> row{};

            // 読み込んだ文字列をストリームに変換
            std::stringstream ss{ line };
            std::string cell;

            // ','区切りで分割
            while (std::getline(ss, cell, delimiter))
            {
                // 読み込んだセルデータを1行分の vector に追加
                row.push_back(cell);
            }

            // 完成した1行分のデータを全体のデータに追加
            data.push_back(row);
        }

        return data;
    }

    static inline std::vector<std::vector<int>> ConvertToInteger(const std::vector<std::vector<std::string>>& stringVector)
    {
        // データ格納用の二次元配列
        std::vector<std::vector<int>> data{};

        // 元の配列の各行(row)変換処理
        for (const auto& row_str : stringVector)
        {
            // 行(row)を格納
            std::vector<int> row{};

            // 現在の行の要素を処理
            for (const auto& letter : row_str)
            {
                try
                {
                    // std::stoi を使用して文字列を整数に変換
                    int converted_int = std::stoi(letter);

                    // 変換結果を現在の行に追加
                    row.push_back(converted_int);
                }
                catch (const std::invalid_argument& err)
                {
                    // 有効な整数でなかった場合-1を入れる
                    row.push_back(-1);
                }
            }

            // 変換が完了した行を結果の二次元ベクトルに追加
            data.push_back(row);
        }

        return data;
    }
};