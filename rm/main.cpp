#include <iostream>
#include <vector>
#include <list>
#include <cstdint>

using namespace std;

vector<uint8_t> interleaving(vector<uint8_t> &sym_din, size_t M, size_t I)
{
    list<uint8_t> sym_fifo[I];
    vector<uint8_t> sym_itlv;

    // Initialize Data FiFo
    for(size_t i = 0; i < I; ++i) {
        for(size_t j = 0; j < M*i; ++j)
            sym_fifo[i].push_back(uint8_t(0));
    }

    // Interleaving
    for(size_t i = 0; i < sym_din.size(); ++i) {
        sym_fifo[size_t(i%I)].push_back(sym_din[i]);
        sym_itlv.push_back(sym_fifo[i%I].front());
        sym_fifo[i%I].pop_front();
    }

    return sym_itlv;
}

vector<uint8_t> deinterleaving_fifo(vector<uint8_t> &sym_din, size_t M, size_t I)
{
    list<uint8_t> sym_fifo[I];
    vector<uint8_t> sym_itlv;

    // Initialize Data FiFo
    for(size_t i = 0; i < I; ++i) {
        for(size_t j = 0; j < M*(I - 1 - i); ++j)
            sym_fifo[i].push_back(uint8_t(0));
    }

    // DeInterleaving
    for(size_t i = 0; i < sym_din.size(); ++i) {
        sym_fifo[size_t(i%I)].push_back(sym_din[i]);
        sym_itlv.push_back(sym_fifo[i%I].front());
        sym_fifo[i%I].pop_front();
    }

    // DeInterleaving Delay Should be I*(I-1)*M
    return sym_itlv;
}

vector<uint8_t> deinterleaving(vector<uint8_t> &sym_din, size_t M, size_t I)
{
    size_t branch = 0;
    size_t wraddr = 0;
    size_t rdaddr = (I - 1) * M;
    vector<uint8_t> sym_ram(I*(I-1)*M/2+1);
    vector<uint8_t> sym_itlv;

    // Initialize Data RAM
    for(size_t i = 0; i < I*(I-1)*M/2 + 1; ++i) {
        sym_ram[i] = uint8_t(0);
    }

    // DeInterleaving
    for(size_t i = 0; i < sym_din.size(); ++i) {
        branch = i % I;

        sym_ram[wraddr] = sym_din[i];
        wraddr = wraddr + (I - 1- branch) * M;
        wraddr = wraddr >= (I*(I-1)*M/2 + 1) ? wraddr - (I*(I-1)*M/2 + 1) : wraddr;

        sym_itlv.push_back(sym_ram[rdaddr]);
        rdaddr = rdaddr + (I - 1 - (branch + 1) % I) * M;
        rdaddr = rdaddr >= (I*(I-1)*M/2 + 1) ? rdaddr - (I*(I-1)*M/2 + 1) : rdaddr;
    }

    // DeInterleaving Delay Should be I*(I-1)*M
    return sym_itlv;
}

int main(int argc, char **argv)
{
    size_t deint_delay = 0;
    size_t I = 12;
    size_t M = 17;
    vector<uint8_t> sym_din;
    vector<uint8_t> sym_itlv;
    vector<uint8_t> sym_deitlv;

    for(size_t i = 0; i < I*(I-1)*M*10; ++i)
        sym_din.push_back(i+1);

    sym_itlv = interleaving(sym_din, M, I);
    sym_deitlv = deinterleaving(sym_itlv, M, I);

    cout << "Interleaving:" << endl;
    for(vector<uint8_t>::iterator itr = sym_itlv.begin(); itr != sym_itlv.end(); ++itr) {
        cout << int(*itr) << ' ' << flush;
    }
    cout << endl;

    cout << "DeInterleaving:" << endl;

    for(vector<uint8_t>::iterator itr = sym_deitlv.begin(); itr != sym_deitlv.end(); ++itr) {
        cout << int(*itr) << ' ' << flush;
        if(*itr == 1 && deint_delay == 0)
            deint_delay = itr - sym_deitlv.begin();
    }
    cout << endl;
    cout << "Deinterleaving Delay is " << deint_delay << endl;

    return 0;
}
