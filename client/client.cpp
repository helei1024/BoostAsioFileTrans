// ConsoleApplication1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include <iostream>
#include <cstdio>
#include <cstring>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "file_info.h"
#include <thread>


void sender(asio::io_service& io, const char* ip_address, unsigned port, const char* filename)
{
	typedef asio::ip::tcp TCP;

	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) {
		std::cerr << "cannot open file\n";
		return;
	}

	//ʹ������ָ�룬��ֹ��������쳣ʱ��fcloseδ�����á�
	boost::shared_ptr<FILE> file_ptr(fp, fclose);

	clock_t cost_time = clock();

	const size_t k_buffer_size = 32 * 1024;
	char buffer[k_buffer_size];
	File_info file_info;

	int filename_size = strlen(filename) + 1;
	size_t file_info_size = sizeof(file_info);
	size_t total_size = file_info_size + filename_size;
	if (total_size > k_buffer_size) {
		std::cerr << "File name is too long";
		return;
	}
	file_info.filename_size = filename_size;

	fseek(fp, 0, SEEK_END);
	file_info.filesize = ftell(fp);
	rewind(fp);

	memcpy(buffer, &file_info, file_info_size);
	memcpy(buffer + file_info_size, filename, filename_size);

	TCP::socket socket(io);
	socket.connect(TCP::endpoint(asio::ip::address_v4::from_string(ip_address), port));

	std::cout << "Sending file : " << filename << "\n";
	size_t len = total_size;
	unsigned long long total_bytes_read = 0;
	while (true) {
		socket.send(asio::buffer(buffer, len), 0);    // ��һ���ȷ��� �ļ���Ϣ�� �ļ�����
		if (feof(fp)) break;
		len = fread(buffer, 1, k_buffer_size, fp);
		total_bytes_read += len;
	}

	cost_time = clock() - cost_time;
	if (cost_time == 0) cost_time = 1;
	double speed = total_bytes_read * (CLOCKS_PER_SEC / 1024.0 / 1024.0) / cost_time;
	std::cout << "cost time: " << cost_time / (double)CLOCKS_PER_SEC << " s "
		<< "  transferred_bytes: " << total_bytes_read << " bytes\n"
		<< "speed: " << speed << " MB/s\n\n";
}

int main(int args, char* argc[])
{
	//if (args < 3) {
	//	std::cerr << "Usage: " << argc[0] << " ip_address  filename1 filename2 \n";
	//	return 1;
	//}

	//asio::io_service io;
	//for (int i = 2; i < args; ++i) {
	//	try { sender(io, argc[1], 1345, argc[i]); }
	//	catch (std::exception& err) {
	//		std::cerr << err.what() << "\n";
	//	}
	//}


	asio::io_service io;
	
		try { sender(io, "127.0.0.1", 8888, "d:\\kkkk.rar"); }
		catch (std::exception& err) {
			std::cerr << err.what() << "\n";
		}

}


//using namespace std;
//using namespace boost;
//using namespace boost::asio;
//
//
//const int thread_num = 10;
//
//int c = 0;
//
//boost::asio::io_service ios;
//
//mutex io_mu;
//
//void func()
//{
//	//io_mu.lock();
//	cout << endl;
//	cout << "-----"<<c++ << endl;
//	cout << endl;
//	//io_mu.unlock();
//}
//
//
//
//int main(int args, char* argc[])
//{
//	boost::asio::io_service ios;
//	for (int a = 0; a < 100; ++a)
//	{
//		//�����������Ͷ�����񣬸����������һ������run�������߳�ִ��
//		ios.post(func);
//
//	}
//	thread *t[thread_num] = { 0 };
//	// �����̳߳�
//	for (int i = 0; i < thread_num; ++i)
//	{
//		
//		t[i] = new thread(boost::bind(&boost::asio::io_service::run, &ios));
//	}
//
//
//	for (int a = 0; a < 100; ++a)
//	{
//		//�����������Ͷ�����񣬸����������һ������run�������߳�ִ��
//		ios.post(func);
//
//	}
//
//	// �ȴ��߳��˳�
//	for (int i = 0; i < thread_num; ++i)
//	{
//		t[i]->join();
//	}
//
//
//
//
//	/*
//	io_service ios[thread_num];
//	// ��ѵͶ������
//	for (int i = 0; i < 10; ++i)
//	{
//		for (int k = 0; k < 10;k++)
//		{ 
//			ios[i].post(func);
//		}
//		//ios[i].post(func);
//	}
//
//	thread *t[thread_num] = { 0 };
//	// �����̳߳�
//	for (int i = 0; i < thread_num; ++i)
//	{
//		t[i] = new thread(bind(&io_service::run, &ios[i]));
//	}
//
//
//	// �ȴ��߳��˳�
//	for (int i = 0; i < thread_num; ++i)
//	{
//		t[i]->join();
//	}
//	*/
//
//
//	Sleep(1001);
//
//}