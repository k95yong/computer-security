#include <fstream>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <iomanip>
#include <stdint.h>
#include <string>
#include<algorithm>
using namespace std;

// bitmap header�� �д� �迭
char bf[sizeof(BITMAPFILEHEADER)];
char bi[sizeof(BITMAPINFOHEADER)];

// bitmap �� ������ �д� �迭
char buf[5000000];

// file I/O
ifstream fin;
ofstream fout;

string msg; // e�ǰ�� input message, d�ǰ�� output message

int main(int argc, char** argv) {
	if (argv[1][0] == 'e') {
		fin.open("origin.bmp", ios::binary);
		
		// error
		if (!fin.is_open()) {
			cout << "file is not opened\n";
			return 0;
		}
		
		fin.seekg(0, ios::end);
		int sz = fin.tellg();
		fin.seekg(0, ios::beg);
		fout.open("stego.bmp", ios::binary);
		
		getline(cin, msg); // message �Է�
		
						   // bitmap�� header �б�
		fin.read(bf, sizeof(BITMAPFILEHEADER));
		fin.read(bi, sizeof(BITMAPINFOHEADER));
		
		// bitmap�� ������ ��Ÿ���� ������ ũ�⸦ ����
		int rbg_sz = sz - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
		
		// bitmap�� ���󰪵��� buf�� ����
		fin.read(buf, rbg_sz);

		// 1���ھ� �ȼ��� �����ϴ� 3���� ����Ʈ�� LSB�� ���� 1bit�� ���ڸ� ��´�.
		// 1���ڴ� 8����Ʈ �ʿ�
		int rbgIdx = 0;
		for (int mIdx = 0; mIdx < msg.size(); mIdx++) {
			int cur = (int)msg[mIdx];
			for (int j = rbgIdx; j < rbgIdx + 8; j++) {
				
				int curLsb = cur % 2;
				cur /= 2;
				buf[j] -= ((int)buf[j] % 2);
				buf[j] += (int)curLsb;
			}
			rbgIdx += 8;
		}

		// �Է��� ��ġ�� �Է��� �������� �� �� �ֵ��� \0�� �Է��Ѵ�.
		int endChar = '\0';
		for (int j = rbgIdx; j < rbgIdx + 8; j++) {
			int curLsb = endChar % 2;
			endChar /= 2;
			buf[j] -= ((int)buf[j] % 2);
			buf[j] += (int)curLsb;
		}
		rbgIdx += 8;

		// stego.bmp���Ͽ� ������� bitmap�� write�Ѵ�.
		fout.write(bf, sizeof(BITMAPFILEHEADER));
		fout.write(bi, sizeof(BITMAPINFOHEADER));
		fout.write(buf, rbg_sz);
	}
	else if (argv[1][0] == 'd') {
		fin.open("stego.bmp", ios::binary);
		
		// error
		if (!fin.is_open()) {
			cout << "file is not opened\n";
			return 0;
		}
		fin.seekg(0, ios::end);
		int sz = fin.tellg();
		fin.seekg(0, ios::beg);
		fin.read(bf, sizeof(BITMAPFILEHEADER));
		fin.read(bi, sizeof(BITMAPINFOHEADER));
		int rbg_sz = sz - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
		fin.read(buf, rbg_sz);
		int rbgIdx = 0;

		// message�� �ʹ� ��� bitmap�� ������ ��� ��� �߻��� ������ �����ϱ� ���� flag
		bool flag = false;

		while(true) {
			char cur = 0;
			for (int j = rbgIdx; j < rbgIdx + 8; j++) {

				// flag�� �̿��� error handling
				if (rbgIdx > rbg_sz) {
					flag = true;
					break;
				}
				int curLsb = ((int)buf[j] % 2);

				// �Է½ÿ� ������ ��Ʈ�� �Ųٷ� �Է������Ƿ� ��Ʈ������ ���� ������� �����Ѵ�.
				curLsb = (curLsb << j - rbgIdx);
				cur += (int)curLsb;
			}
			if (flag)
				break;
			rbgIdx += 8;
			// ���� ���ڿ��� ���� �˸���� �۵��� �����Ѵ�.
			if (cur == '\0')
				break;
			msg.push_back(cur);
		}
		// ���� �޼��� ���
		cout << msg << '\n';
	}
	return 0;
}