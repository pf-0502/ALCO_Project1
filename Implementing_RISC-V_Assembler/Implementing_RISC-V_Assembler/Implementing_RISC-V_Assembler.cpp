//Project 1 Implementing RISC-V Assembler
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <bitset>
#include <map>

using namespace std;

struct instruction {
	string opcode;
	int rd;
	string func3;
	int rs1;
	int rs2;
	string func7;
	int imm12;
	int imm20;
	int offset;
	int shamt;
};

void type_U(string data, instruction& code);
void type_UJ(string data, instruction& code, int index);
void type_I(string data, instruction& code);
void type_SB(string data, instruction& code, int index);
void type_Il(string data, instruction& code);
void type_S(string data, instruction& code);
void type_Is(string data, instruction& code);
void type_R(string data, instruction& code);

map< string, int > Label;

int main() {
	string data;
	string operation;
	instruction code;

	vector< string > ISA; // 去除掉label的instruction
	//vector< string > show; // show input
	fstream test("test.txt", ios::in);
	for (int i = 0; !test.eof(); i++)
	{
		string input, label, nonlabel;
		stringstream ss, su;
		getline(test, input);
		//show.push_back(input);
		ss << input;
		ss >> label;
		if (label[label.size() - 1] == ':')
		{
			label.pop_back();
			Label.insert(pair<string, int>(label, --i));
			getline(ss, nonlabel);
			ISA.push_back(nonlabel);
		}
		else
			ISA.push_back(input);
	}

	for (int i = 0; i < ISA.size(); i++)
	{
		//先切opcode判斷屬於哪一個type
		stringstream ss;
		ss << ISA[i];
		//cout << show[i] << endl; // show assembly language
		ss >> operation;
		ss >> data;

		//給對應的opcode並傳剩下的參數
		if (operation == "lui")
		{
			code.opcode = "0110111";
			type_U(data, code);
		}
		else if (operation == "auipc")
		{
			code.opcode = "0010111";
			type_U(data, code);
		}
		else if (operation == "jal")
		{
			code.opcode = "1101111";
			type_UJ(data, code, i);
		}
		else if (operation == "jalr")
		{
			code.opcode = "1100111";
			code.func3 = "000";
			type_I(data, code);
		}
		else if (operation == "beq")
		{
			code.opcode = "1100011";
			code.func3 = "000";
			type_SB(data, code, i);
		}
		else if (operation == "bne")
		{
			code.opcode = "1100011";
			code.func3 = "001";
			type_SB(data, code, i);
		}
		else if (operation == "blt")
		{
			code.opcode = "1100011";
			code.func3 = "100";
			type_SB(data, code, i);
		}
		else if (operation == "bge")
		{
			code.opcode = "1100011";
			code.func3 = "101";
			type_SB(data, code, i);
		}
		else if (operation == "bltu")
		{
			code.opcode = "1100011";
			code.func3 = "110";
			type_SB(data, code, i);
		}
		else if (operation == "bgeu")
		{
			code.opcode = "1100011";
			code.func3 = "111";
			type_SB(data, code, i);
		}
		else if (operation == "lb")
		{
			code.opcode = "0000011";
			code.func3 = "000";
			type_Il(data, code);
		}
		else if (operation == "lh")
		{
			code.opcode = "0000011";
			code.func3 = "001";
			type_Il(data, code);
		}
		else if (operation == "lw")
		{
			code.opcode = "0000011";
			code.func3 = "010";
			type_Il(data, code);
		}
		else if (operation == "lbu")
		{
			code.opcode = "0000011";
			code.func3 = "100";
			type_Il(data, code);
		}
		else if (operation == "lhu")
		{
			code.opcode = "0000011";
			code.func3 = "101";
			type_Il(data, code);
		}
		else if (operation == "sb")
		{
			code.opcode = "0100011";
			code.func3 = "000";
			type_S(data, code);
		}
		else if (operation == "sh")
		{
			code.opcode = "0100011";
			code.func3 = "001";
			type_S(data, code);
		}
		else if (operation == "sw")
		{
			code.opcode = "0100011";
			code.func3 = "010";
			type_S(data, code);
		}
		else if (operation == "addi")
		{
			code.opcode = "0010011";
			code.func3 = "000";
			type_I(data, code);
		}
		else if (operation == "slti")
		{
			code.opcode = "0010011";
			code.func3 = "010";
			type_I(data, code);
		}
		else if (operation == "sltiu")
		{
			code.opcode = "0010011";
			code.func3 = "011";
			type_I(data, code);
		}
		else if (operation == "xori")
		{
			code.opcode = "0010011";
			code.func3 = "100";
			type_I(data, code);
		}
		else if (operation == "ori")
		{
			code.opcode = "0010011";
			code.func3 = "110";
			type_I(data, code);
		}
		else if (operation == "andi")
		{
			code.opcode = "0010011";
			code.func3 = "111";
			type_I(data, code);
		}
		else if (operation == "slli")
		{
			code.opcode = "0010011";
			code.func3 = "001";
			code.func7 = "0000000";
			type_Is(data, code);
		}
		else if (operation == "srli")
		{
			code.opcode = "0010011";
			code.func3 = "101";
			code.func7 = "0000000";
			type_Is(data, code);
		}
		else if (operation == "srai")
		{
			code.opcode = "0010011";
			code.func3 = "101";
			code.func7 = "0100000";
			type_Is(data, code);
		}
		else if (operation == "add")
		{
			code.opcode = "0110011";
			code.func3 = "000";
			code.func7 = "0000000";
			type_R(data, code);
		}
		else if (operation == "sub")
		{
			code.opcode = "0110011";
			code.func3 = "000";
			code.func7 = "0100000";
			type_R(data, code);
		}
		else if (operation == "sll")
		{
			code.opcode = "0110011";
			code.func3 = "001";
			code.func7 = "0000000";
			type_R(data, code);
		}
		else if (operation == "slt")
		{
			code.opcode = "0110011";
			code.func3 = "010";
			code.func7 = "0000000";
			type_R(data, code);
		}
		else if (operation == "sltu")
		{
			code.opcode = "0110011";
			code.func3 = "011";
			code.func7 = "0000000";
			type_R(data, code);
		}
		else if (operation == "xor")
		{
			code.opcode = "0110011";
			code.func3 = "100";
			code.func7 = "0000000";
			type_R(data, code);
		}
		else if (operation == "srl")
		{
			code.opcode = "0110011";
			code.func3 = "101";
			code.func7 = "0000000";
			type_R(data, code);
		}
		else if (operation == "sra")
		{
			code.opcode = "0110011";
			code.func3 = "101";
			code.func7 = "0100000";
			type_R(data, code);
		}
		else if (operation == "or")
		{
			code.opcode = "0110011";
			code.func3 = "110";
			code.func7 = "0000000";
			type_R(data, code);
		}
		else if (operation == "and")
		{
			code.opcode = "0110011";
			code.func3 = "111";
			code.func7 = "0000000";
			type_R(data, code);
		}
	}
}

void type_U(string data, instruction& code)
{
	stringstream su;
	string temp1, temp2;
	su << data;
	getline(su, temp1, ',');
	getline(su, temp2);
	su.str("");
	su.clear();

	temp1 = temp1.substr(1, temp1.size() - 1);
	code.rd = stoi(temp1);
	code.imm20 = stoi(temp2);
	bitset<5> rd(code.rd);
	bitset<20> imm(code.imm20);

	cout << imm << rd << code.opcode << endl;
}

void type_UJ(string data, instruction& code, int index)
{
	stringstream su;
	string temp1, temp2, simm, simm20, simm10_1, simm11, simm19_12;
	su << data;
	getline(su, temp1, ',');
	getline(su, temp2);
	su.str("");
	su.clear();

	temp1 = temp1.substr(1, temp1.size() - 1);
	code.rd = stoi(temp1);

	map< string, int >::iterator it;
	it = Label.find(temp2);

	code.offset = (it->second - index) * 4; // label與目前instruction差多少byte的address
	bitset<5> rd(code.rd);
	bitset<21> imm(code.offset);

	simm = imm.to_string();
	simm20 = simm.substr(0, 1); // 20
	simm10_1 = simm.substr(10, 10); // 10:1
	simm11 = simm.substr(9, 1); // 11
	simm19_12 = simm.substr(1, 8); // 19:12

	cout << simm20 << simm10_1 << simm11 << simm19_12 << rd << code.opcode << endl;
}

void type_I(string data, instruction& code)
{
	stringstream su;
	string temp1, temp2, temp3;
	su << data;
	getline(su, temp1, ',');
	getline(su, temp2, ',');
	getline(su, temp3);
	su.str("");
	su.clear();

	temp1 = temp1.substr(1, temp1.size() - 1);
	temp2 = temp2.substr(1, temp2.size() - 1);
	code.rd = stoi(temp1);
	code.rs1 = stoi(temp2);
	code.imm12 = stoi(temp3);
	bitset<5> rd(code.rd);
	bitset<5> rs1(code.rs1);
	bitset<12> imm(code.imm12);

	cout << imm << rs1 << code.func3 << rd << code.opcode << endl;
}

void type_SB(string data, instruction& code, int index)
{
	stringstream su;
	string temp1, temp2, temp3, simm, simm10_5, simm4_1, simm11, simm12;
	su << data;
	getline(su, temp1, ',');
	getline(su, temp2, ',');
	getline(su, temp3);
	su.str("");
	su.clear();

	temp1 = temp1.substr(1, temp1.size() - 1);
	temp2 = temp2.substr(1, temp2.size() - 1);
	code.rs1 = stoi(temp1);
	code.rs2 = stoi(temp2);

	map< string, int >::iterator it;
	it = Label.find(temp3);

	code.offset = (it->second - index) * 4; // label與目前instruction差多少byte的address

	bitset<5> rs1(code.rs1);
	bitset<5> rs2(code.rs2);
	bitset<13> offset(code.offset);

	simm = offset.to_string();
	simm12 = simm.substr(0, 1); // 12
	simm10_5 = simm.substr(2, 6); // 10:5
	simm4_1 = simm.substr(8, 4); // 4:1
	simm11 = simm.substr(1, 1); // 11

	cout << simm12 << simm10_5 << rs2 << rs1 << code.func3 << simm4_1 << simm11 << code.opcode << endl;
}

void type_Il(string data, instruction& code)
{
	stringstream su;
	string temp1, temp2, temp3;
	su << data;
	getline(su, temp1, ',');
	getline(su, temp2, '(');
	getline(su, temp3, ')');
	su.str("");
	su.clear();

	temp1 = temp1.substr(1, temp1.size() - 1);
	temp3 = temp3.substr(1, temp3.size() - 1);
	code.rd = stoi(temp1);
	code.offset = stoi(temp2);
	code.rs1 = stoi(temp3);
	bitset<5> rd(code.rd);
	bitset<5> rs1(code.rs1);
	bitset<12> offset(code.offset);

	cout << offset << rs1 << code.func3 << rd << code.opcode << endl;
}

void type_S(string data, instruction& code)
{
	stringstream su;
	string temp1, temp2, temp3, simm, simm11_5, simm4_0;
	su << data;
	getline(su, temp1, ',');
	getline(su, temp2, '(');
	getline(su, temp3, ')');
	su.str("");
	su.clear();

	temp1 = temp1.substr(1, temp1.size() - 1);
	temp3 = temp3.substr(1, temp3.size() - 1);
	code.rs2 = stoi(temp1);
	code.offset = stoi(temp2);
	code.rs1 = stoi(temp3);
	bitset<5> rs2(code.rs2);
	bitset<5> rs1(code.rs1);
	bitset<12> offset(code.offset);

	simm = offset.to_string();
	simm11_5 = simm.substr(0, 7);
	simm4_0 = simm.substr(7, 5);

	cout << simm11_5 << rs2 << rs1 << code.func3 << simm4_0 << code.opcode << endl;
}

void type_Is(string data, instruction& code)
{
	stringstream su;
	string temp1, temp2, temp3;
	su << data;
	getline(su, temp1, ',');
	getline(su, temp2, ',');
	getline(su, temp3);
	su.str("");
	su.clear();

	temp1 = temp1.substr(1, temp1.size() - 1);
	temp2 = temp2.substr(1, temp2.size() - 1);
	code.rd = stoi(temp1);
	code.rs1 = stoi(temp2);
	code.shamt = stoi(temp3);
	bitset<5> rd(code.rd);
	bitset<5> rs1(code.rs1);
	bitset<5> imm(code.shamt);

	cout << code.func7 << imm << rs1 << code.func3 << rd << code.opcode << endl;
}

void type_R(string data, instruction& code)
{
	stringstream su;
	string temp1, temp2, temp3;
	su << data;
	getline(su, temp1, ',');
	getline(su, temp2, ',');
	getline(su, temp3);
	su.str("");
	su.clear();

	temp1 = temp1.substr(1, temp1.size() - 1);
	temp2 = temp2.substr(1, temp2.size() - 1);
	temp3 = temp3.substr(1, temp3.size() - 1);
	code.rd = stoi(temp1);
	code.rs1 = stoi(temp2);
	code.rs2 = stoi(temp3);
	bitset<5> rd(code.rd);
	bitset<5> rs1(code.rs1);
	bitset<5> rs2(code.rs2);

	cout << code.func7 << rs2 << rs1 << code.func3 << rd << code.opcode << endl;
}