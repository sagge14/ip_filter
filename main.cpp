
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>


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

        auto printIP = [](auto& ip)
        {
            for(auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part)
            {
                if (ip_part != ip.cbegin())
                    std::cout << ".";
                std::cout << *ip_part;
            }
            std::cout << std::endl;
        };
        auto comp = []( auto& a, auto& b)
        {
            std::tuple<uint8_t,uint8_t,uint8_t,uint8_t> ip1,ip2;
            auto myTuple = [](auto& i){
                return std::make_tuple(std::atoi(i.at(0).c_str()),
                                       std::atoi(i.at(1).c_str()),
                                       std::atoi(i.at(2).c_str()),
                                       std::atoi(i.at(3).c_str()));
            };

            ip1 = myTuple(a);
            ip2 = myTuple(b);

           return ip1 > ip2;
        };
        auto filter = [&ip_pool, printIP](size_t n)
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
        auto filter_any = [&ip_pool, printIP](size_t n)
        {
            for(const auto& ip:ip_pool)
                if(std::any_of(ip.cbegin(),ip.cend(),[n](auto& i){return std::atoi(i.c_str()) == n;}))
                    printIP(ip);
        };

        //for(std::string line; std::getline(std::cin, line) ;)
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

        // TODO filter by first byte and output

        filter(1);

        // TODO filter by first and second bytes and output
        filter2(46, 70);

        // TODO filter by any byte and output
        filter_any(46);

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    std::getchar();
    return 0;
}
