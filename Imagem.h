#pragma once
#include <string>
#include <vector>

typedef unsigned char byte;

class Imagem {
public:
	Imagem(std::string nome,std::string extensao, int altura, int largura);
	~Imagem();
	//void alterar_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	//void aplicar_filtro(const std::string& tipo);

	//std::string getInfo() const;

	int imSave(std::string nome, std::string extensao);
	int imLoad(std::string nome, std::string extensao);
	int altura, largura;
	void setNome(std::string nome);
	std::string getNome();
	void setExtensao(std::string extensao_);
	std::string getExtensao();
	float* dados;
	unsigned char* dados_byte;
	bool Alpha_ativado = false;

private:
	std::string nome;
	std::string extensao;
	std::string data_criacao;
	std::string data_ultima_alteracao;
	std::string cor_predominante;

};