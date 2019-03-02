#include <iostream>
#include <fstream>
using namespace std;

#define MAX 100

struct Canh
{
    int u;
    int v;
    int w;
};

// đọc đồ thị được lưu dưới dạng ma trận:
// 3
// 0 1 2
// 1 0 3
// 2 3 0
void docDoThi(int maTran[][MAX], int &bac, const char path[])
{
    ifstream df(path);
    df >> bac;
    for (int i = 0; i < bac; i++)
        for (int j = 0; j < bac; j++)
            df >> maTran[i][j];
    df.close();
}

// xuất ma trận ra console
void xuatMaTran(int maTran[][MAX], const int bac)
{
    cout << "Ma tran co " << bac << " dinh :" << endl;
    for (int i = 0; i < bac; i++)
    {
        cout << "\t";
        for (int j = 0; j < bac; j++)
            cout << maTran[i][j] << "\t";
        cout << endl;
    }
}

// xuất cây khung ra console
void xuatCayKhung(Canh canh[], const int soDinh)
{
    int doDai = 0; // độ dài cây khung
    cout << "cay khung co cac canh la:" << endl;
    for (int i = 0; i < soDinh - 1; i++)
    {
        cout << '\t' << '[' << canh[i].u << ']' << "--" << canh[i].w << "--" << '[' << canh[i].v << ']' << endl;
        doDai += canh[i].w;
    }
    cout << "do danh cay khung la: " << doDai;
}

// lưu cây khung xuống file
int luuCayKhung(Canh canh[], const int soDinh, const char duongDanKetQua[])
{
    ofstream gf(duongDanKetQua);
    int doDai = 0; // độ dài cây khung
    gf << "cay khung co cac canh la:" << endl;
    for (int i = 0; i < soDinh - 1; i++)
    {
        gf << '\t' << '[' << canh[i].u << ']' << "--" << canh[i].w << "--" << '[' << canh[i].v << ']' << endl;
        doDai += canh[i].w;
    }
    gf << "do danh cay khung la: " << doDai;
    gf.close();
}

/// <---------- Prim BEGIN ---------->

// thêm đỉnh kề gần nhất vào cây khung
bool themDinhKeGanNhat(int maTran[][MAX], const int bac, bool daDuyet[], Canh canh[], int &soCanh)
{
    int w = INT_MAX; // quảng đường
    int u;           // đỉnh
    int v = -1;      // đỉnh

    for (int i = 0; i < bac; i++)
    {
        if (!daDuyet[i])
        {
            // đỉnh chưa có trong cây khung
            continue;
        }
        for (int j = 0; j < bac; j++)
        {
            if (daDuyet[j])
            {
                // đỉnh đã có trong cây khung
                continue;
            }
            else if (maTran[i][j] > 0 && maTran[i][j] < w)
            {
                // đỉnh chưa có trong cây khung và có quảng đường ngắn hơn quảng đường hiện tại
                // lưu quảng đường ngắn nhất và đỉnh
                w = maTran[i][j];
                u = i;
                v = j;
            }
        }
    }

    if (v != -1)
    {
        // tìm thấy đỉnh để thêm vào cây khung
        daDuyet[v] = true;
        canh[soCanh].u = u;
        canh[soCanh].v = v;
        canh[soCanh].w = w;
        soCanh++;
        return true;
    }
    else
    {
        // không tìm thấy đỉnh để thêm vào cây khung
        return false;
    }
}

// dựng cây khung bằng giải thuật Prim
bool dungCayKhungPrim(int maTran[][MAX], const int bac, const int dinhXuatPhat, Canh canh[])
{
    Canh canhCay[MAX]; // các cạnh trong cây khung
    int soCanh = 0;    // số cạnh trong cây khung
    bool daDuyet[MAX]; // đánh dấu các đỉnh đã duyệt

    daDuyet[dinhXuatPhat] = 1;
    for (int i = 0; i < bac - 1; i++)
    {
        if (!themDinhKeGanNhat(maTran, bac, daDuyet, canh, soCanh))
        {
            // thêm đỉnh thất bại
            return false;
        }
    }
    // dựng cây hoàn tất
    return true;
}

/// <---------- Prim END ---------->

/// <---------- Kruskal BEGIN ---------->
// lấy danh sách cạnh từ ma trận
void layCanh(int maTran[][MAX], const int bac, Canh canh[], int &soCanh)
{
    soCanh = 0;
    for (int i = 0; i < bac; i++)
    {
        for (int j = 0; j < bac; j++)
        {
            if (maTran[i][j] > 0)
            {
                canh[soCanh].u = i;
                canh[soCanh].v = j;
                canh[soCanh].w = maTran[i][j];
                soCanh++;
            }
        }
    }
}

// sắp xếp cạnh theo thứ tự độ dài tăng dần
void xepCanh(Canh canh[], int &soCanh)
{
    for (int i = 0; i < soCanh; i++)
    {
        for (int j = soCanh - 1; j > i; j--)
        {
            if (canh[j].w < canh[j - 1].w)
            {
                Canh temp = canh[j];
                canh[j] = canh[j - 1];
                canh[j - 1] = canh[j];
            }
        }
    }
}

// tìm đỉnh gốc của 1 đỉnh
int timGoc(int cha[], int u)
{
    if (cha[u] == u)
    {
        return u;
    }
    else
    {
        return cha[u] = timGoc(cha, u);
    }
}

// kiểm tra 2 đỉnh u,v có chung gốc không
bool coChungGoc(int cha[], int u, int v)
{
    return timGoc(cha, u) == timGoc(cha, v);
}

// nối 2 cây con lại
void noiCay(int cha[], int u, int v)
{
    if (!coChungGoc(cha, u, v))
    {
        const int x = timGoc(cha, u);
        const int y = timGoc(cha, v);
        cha[y] = x;
    }
}

// dựng cây khung bằng giải thuật Kruskal
void dungCayKhungKruskal(int maTran[][MAX], const int bac, const int dinhXuatPhat, Canh dsCanhCay[])
{
    Canh dsCanh[MAX];  // danh sách các cạnh trong đồ thì
    int soCanh;        // số cạnh trong đồ thị
    int soCanhCay = 0; // số cạnh của cây khung hiện tại.
    int cha[MAX];      // danh sách cha của cây con

    for (int i = 0; i < bac; i++)
    {
        cha[i] = i;
    }

    layCanh(maTran, bac, dsCanh, soCanh);

    xepCanh(dsCanh, soCanh);

    for (int i = 0; i < soCanh && soCanhCay < bac - 1; i++)
    {
        if (coChungGoc(cha, dsCanh[i].u, dsCanh[i].v))
        {
            noiCay(cha, dsCanh[i].u, dsCanh[i].v);
            dsCanhCay[soCanhCay] = dsCanh[i];
            soCanhCay++;
        }
    }
}
/// <---------- Kruskal END ---------->

int main()
{
    cout << "Chuong trinh tim cay khung nho nhat!\n";
    //  khai báo và khởi tạo biến
    int soDinh;          // bậc ma trận NxN
    int doThi[MAX][MAX]; // ma trận của đồ thị
    // đường dẫn tới tập tin ma trận
    const char duongDanMaTran[] = ("./input.txt");
    // cây khung
    bool ketQua;       // kết quả dựng cây khung
    Canh canhCay[MAX]; // các cạnh trong cây khung
    // đường dẫn lưu kết quả
    const char duongDanKetQua[] = ("./output.txt");

    // đọc ma trận và bậc ma trận từ đường đường dẫn
    docDoThi(doThi, soDinh, duongDanMaTran);

    xuatMaTran(doThi, soDinh);

    // <---------- Prim BEGIN ---------->
    ketQua = dungCayKhungPrim(doThi, soDinh, 0, canhCay);
    if (!ketQua)
    {
        // không dựng được cây.
        cout << " khong dung duoc cay khung.";
        return -1;
    }
    xuatCayKhung(canhCay, soDinh);

    // <---------- Prim END ---------->

    // <---------- Kruskal BEGIN ---------->
    dungCayKhungKruskal(doThi, soDinh, 0, canhCay);
    xuatCayKhung(canhCay, soDinh);
    // <---------- Kruskal END ---------->

    luuCayKhung(canhCay, soDinh, duongDanKetQua);
}
