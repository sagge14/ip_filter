#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>

// ("",  '.') -> [""] 123
// ("11", '.') -> ["11"]
// ("..", '.')ffff -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

int main(int argc, char const *argv[])
{
    try
    {

        std::fstream f("ip_filter.tsv");
        std::vector<std::vector<std::string>> ip_pool;
        typedef decltype(*ip_pool.cbegin()) myIt;


        auto printIP = [](myIt& ip)
        {
            for(auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part)
            {
                if (ip_part != ip.cbegin())
                {
                    std::cout << ".";

                }
                std::cout << *ip_part;
            }
            std::cout << std::endl;
        };
        auto comp = []( myIt& a, myIt& b)
        {
            std::tuple<uint8_t,uint8_t,uint8_t,uint8_t> ip1,ip2;
            auto myTuple = [](myIt& i){
                return std::make_tuple(std::atoi(i.at(0).c_str()),
                                       std::atoi(i.at(1).c_str()),
                                       std::atoi(i.at(2).c_str()),
                                       std::atoi(i.at(3).c_str()));
            };

            ip1 = myTuple(a);
            ip2 = myTuple(b);

            if(std::get<0>(ip1) > std::get<0>(ip2))
                return true;

            if(std::get<0>(ip1) == std::get<0>(ip2) && std::get<1>(ip1) > std::get<1>(ip2))
                return true;

            if(std::get<1>(ip1) == std::get<1>(ip2) && std::get<2>(ip1) > std::get<2>(ip2))
                return true;

            if(std::get<2>(ip1) == std::get<2>(ip2) && std::get<3>(ip1) > std::get<3>(ip2))
                return true;

             return false;
        };
        auto filter = [&ip_pool,printIP](size_t n)
        {
            for(const auto& ip:ip_pool)
                if(std::atoi(ip.at(0).c_str()) == n)
                    printIP(ip);
        };
        auto filter2 = [&ip_pool,printIP](size_t a, size_t b)
        {
            for(const auto& ip:ip_pool)
                if(std::atoi(ip.at(0).c_str()) == a && std::atoi(ip.at(1).c_str()) == b)
                    printIP(ip);
        };
        auto filter_any = [&ip_pool,printIP](size_t n)
        {
            for(const auto& ip:ip_pool)
            {
                if(std::any_of(ip.cbegin(),ip.cend(),[n](auto& i){return std::atoi(i.c_str()) == n;}))
                    printIP(ip);
            }
        };

        for(std::string line; std::getline(f, line) ;)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        f.close();

        // TODO reverse lexicographically sort

        std::sort(ip_pool.begin(),ip_pool.end(),comp);

        for(const auto & ip : ip_pool)
            printIP(ip);

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output

        filter(1);
        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        filter2(46, 70);

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        filter_any(46);

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    system("pause");
    return 0;
}
