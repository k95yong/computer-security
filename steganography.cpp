#include <fstream>
#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <iomanip>
#include <stdint.h>
#include <string>
#include<algorithm>
using namespace std;

// bitmap header를 읽는 배열
char bf[sizeof(BITMAPFILEHEADER)];
char bi[sizeof(BITMAPINFOHEADER)];

// bitmap 의 색상값을 읽는 배열
char buf[5000000];

// file I/O
ifstream fin;
ofstream fout;

string msg; // e의경우 input message, d의경우 output message

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
		
		getline(cin, msg); // message 입력
		
						   // bitmap의 header 읽기
		fin.read(bf, sizeof(BITMAPFILEHEADER));
		fin.read(bi, sizeof(BITMAPINFOHEADER));
		
		// bitmap의 색상을 나타내는 영역의 크기를 구함
		int rbg_sz = sz - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
		
		// bitmap의 색상값들을 buf에 읽음
		fin.read(buf, rbg_sz);

		// 1글자씩 픽셀을 구성하는 3개의 바이트의 LSB에 각각 1bit씩 글자를 담는다.
		// 1글자당 8바이트 필요
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

		// 입력을 마치고 입력이 끝났음을 알 수 있도록 \0을 입력한다.
		int endChar = '\0';
		for (int j = rbgIdx; j < rbgIdx + 8; j++) {
			int curLsb = endChar % 2;
			endChar /= 2;
			buf[j] -= ((int)buf[j] % 2);
			buf[j] += (int)curLsb;
		}
		rbgIdx += 8;

		// stego.bmp파일에 만들어진 bitmap을 write한다.
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

		// message가 너무 길어 bitmap의 범위를 벗어날 경우 발생할 에러를 방지하기 위한 flag
		bool flag = false;

		while(true) {
			char cur = 0;
			for (int j = rbgIdx; j < rbgIdx + 8; j++) {

				// flag를 이용한 error handling
				if (rbgIdx > rbg_sz) {
					flag = true;
					break;
				}
				int curLsb = ((int)buf[j] % 2);

				// 입력시에 글자의 비트를 거꾸로 입력했으므로 비트연산을 통해 원래대로 복원한다.
				curLsb = (curLsb << j - rbgIdx);
				cur += (int)curLsb;
			}
			if (flag)
				break;
			rbgIdx += 8;
			// 현재 문자열이 끝을 알릴경우 작동을 중지한다.
			if (cur == '\0')
				break;
			msg.push_back(cur);
		}
		// 최종 메세지 출력
		cout << msg << '\n';
	}
	return 0;
}