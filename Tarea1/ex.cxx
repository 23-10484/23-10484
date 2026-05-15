#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<char> dec_to_base(int n, int base){
    if(n == 0) return {'0'};

    vector<char> result;

    while(n > 0){
        int digit = n % base;

        if(digit < 10){
            result.push_back('0' + digit);
        }else{
            result.push_back('A' + (digit - 10));
        }

        n /= base;
    }

    reverse(result.begin(), result.end());

    return result;
}

int base_to_dec_int(vector<char> s, int base){
    int result = 0;

    for(char c : s){
        int digit;

        if(c >= '0' && c <= '9'){
            digit = c - '0';
        }else{
            digit = c - 'A' + 10;
        }

        result = result * base + digit;
    }

    return result;
}

vector<char> int_to_vector(int n){
    return dec_to_base(n, 10);
}

vector<char> dec_to_septapus(int n){
    return dec_to_base(n, 7);
}

vector<char> dec_to_octopus(int n){
    return dec_to_base(n, 8);
}

vector<char> dec_to_hexakaidecapus(int n){
    return dec_to_base(n, 16);
}

vector<char> septapus_to_dec(vector<char> s){
    return int_to_vector(base_to_dec_int(s, 7));
}

vector<char> octopus_to_dec(vector<char> s){
    return int_to_vector(base_to_dec_int(s, 8));
}

vector<char> hexakaidecapus_to_dec(vector<char> s){
    return int_to_vector(base_to_dec_int(s, 16));
}

vector<char> septapus_to_octopus(vector<char> s){
    int n = base_to_dec_int(s, 7);
    return dec_to_octopus(n);
}

vector<char> septapus_to_hexakaidecapus(vector<char> s){
    int n = base_to_dec_int(s, 7);
    return dec_to_hexakaidecapus(n);
}

vector<char> octapus_to_septapus(vector<char> s){
    int n = base_to_dec_int(s, 8);
    return dec_to_septapus(n);
}

vector<char> octopus_to_hexakaidecapus(vector<char> s){
    int n = base_to_dec_int(s, 8);
    return dec_to_hexakaidecapus(n);
}

vector<char> hexakaidecapus_to_septapus(vector<char> s){
    int n = base_to_dec_int(s, 16);
    return dec_to_septapus(n);
}

vector<char> hexakaidecapus_to_octopus(vector<char> s){
    int n = base_to_dec_int(s, 16);
    return dec_to_octopus(n);
}
