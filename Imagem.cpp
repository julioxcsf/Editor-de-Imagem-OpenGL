#include <iostream>
#include <fstream>
#include <sstream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "Imagem.h"


Imagem::Imagem(const std::string nome, const std::string extensao, int altura, int largura)
	: nome(nome), extensao(extensao), altura(altura), largura(largura) {
	std::cout << "Nome: " << nome << std::endl;
	std::cout << "Extensao: " << extensao << std::endl;
	std::cout << "Altura: " << altura << std::endl;
	std::cout << "Largura: " << largura << std::endl;
	dados = new float[altura * largura * 4];
	dados_byte = new unsigned char[altura * largura * 4];
	std::cout << "Objeto da classe Imagem gerado.\n" << std::endl;
}

Imagem::~Imagem() {
	// Libera a memória alocada para a matriz de pixels
	delete[] dados;
	delete[] dados_byte;
}

void saveAsPNG(const std::string& filename, int width, int height, const unsigned char* data) {
	stbi_write_png(filename.c_str(), width, height, 4, data, width * 4);
}

void saveAsJPEG(const std::string& filename, int width, int height, const unsigned char* data) {
	stbi_write_jpg(filename.c_str(), width, height, 4, data, 100); // 100 é a qualidade
}

int Imagem::imSave(std::string nome, std::string extensao) {
	std::string caminho = nome + extensao;

	if (extensao == ".raw2") {
		std::ofstream imagefile(caminho);

		// Verifica se o arquivo foi aberto corretamente
		if (!imagefile) {
			std::cerr << "Nao foi possivel criar " << caminho << std::endl;
			return 1;
		}

		//sem comentarios no momento
		//imagefile << "# Arquivo " << caminho << std::endl;
		//imagefile << "# Resolucao: " << gerarResolucao(largura, altura) << std::endl;
		//imagefile << "# data: 16/08/2024\n" << std::endl;

		//eu preciso extrair os valores de linha e coluna da string resolucao! ajuda!!!! 
		int i = 0;
		int tamanho = altura * largura * 4;
		int col = 0;
		while (i < tamanho) {
			//considerando dados uma matriz de i,j com 3 ou 4 bytes por pixel
			imagefile << static_cast<int>(dados[i] * 255) << ",";//R
			imagefile << static_cast<int>(dados[i + 1] * 255) << ",";//G
			imagefile << static_cast<int>(dados[i + 2] * 255) << ",";//B
			imagefile << static_cast<int>(dados[i + 3] * 255);//Alpha
			//imagefile << " ";//espaçamento entre valores
			i += 4;
			col++;
			if (col == largura) {
				imagefile << "\n";//espaçamento do RAW
				col = 0;
			}
			else {
				imagefile << " ";//espaçamento entre valores
			}
		}

		// Fecha o arquivo
		imagefile.close();

		// Mensagem de sucesso
		std::cout << "Imagem foi salva como " << caminho << std::endl;
	}
	else if (extensao == ".raw") {
			std::ofstream imagefile(caminho, std::ios::binary);

			if (!imagefile) {
				std::cerr << "Nao foi possivel criar " << caminho << std::endl;
				return 1;
			}

			for (int i = 0; i < altura * largura * 4; ++i) {
				unsigned char colorValue = static_cast<unsigned char>(dados[i] * 255);
				imagefile.write(reinterpret_cast<char*>(&colorValue), sizeof(colorValue));
			}

			imagefile.close();
			std::cout << "Imagem foi salva como " << caminho << std::endl;
	
	}
	/*else if (extensao == ".png") {
		/*int i = 0;
		int tamanho = altura * largura * 4;
		while (i < tamanho) {
			//considerando dados uma matriz de i,j com 3 ou 4 bytes por pixel
			dados_byte[i] = static_cast<int>(dados[i] * 255);//R
			dados_byte[i+1] = static_cast<int>(dados[i + 1] * 255);//G
			dados_byte[i+2] = static_cast<int>(dados[i + 2] * 255);//B
			dados_byte[i+3] = static_cast<int>(dados[i + 3] * 255);//Alpha
			i += 4;
		}*//*
		for (int i = 0; i < altura * largura * 4; ++i) {
			unsigned char colorValue = static_cast<unsigned char>(dados[i] * 255);
			dados_byte[i] = colorValue;
		}
		saveAsPNG(caminho, largura, altura, &dados_byte[0]);
	}
	else if (extensao == ".jpg") {
		int i = 0;
		int tamanho = altura * largura * 4;
		while (i < tamanho) {
			//considerando dados uma matriz de i,j com 3 ou 4 bytes por pixel
			dados_byte[i] = static_cast<unsigned char>(dados[i] * 255);//R
			dados_byte[i + 1] = static_cast<unsigned char>(dados[i + 1] * 255);//G
			dados_byte[i + 2] = static_cast<unsigned char>(dados[i + 2] * 255);//B
			dados_byte[i + 3] = static_cast<unsigned char>(dados[i + 3] * 255);//Alpha
			i += 4;
		}
		saveAsJPEG(caminho, largura, altura, dados_byte);
	}*/
	else if (extensao == ".png" || extensao == ".jpg") {
		int index = 0;
		for (int y = altura - 1; y >= 0; --y) {  // De baixo para cima
			for (int x = 0; x < largura; ++x) {
				int originalIndex = (y * largura + x) * 4;
				dados_byte[index] = static_cast<unsigned char>(dados[originalIndex] * 255);       // R
				dados_byte[index + 1] = static_cast<unsigned char>(dados[originalIndex + 1] * 255); // G
				dados_byte[index + 2] = static_cast<unsigned char>(dados[originalIndex + 2] * 255); // B
				dados_byte[index + 3] = static_cast<unsigned char>(dados[originalIndex + 3] * 255); // A
				index += 4;
			}
		}

		if (extensao == ".png") {
			saveAsPNG(caminho, largura, altura, dados_byte);
		}
		else if (extensao == ".jpg") {
			saveAsJPEG(caminho, largura, altura, dados_byte);
		}
	}
	else {
		std::cout << "Formato nao implementado" << std::endl;
	}
	return 0;
}

int Imagem::imLoad(std::string nome, std::string extensao) {
	std::string caminho = nome + extensao;


	if (extensao == ".raw") {
		std::ifstream imagefile(caminho);

		// Verifica se o arquivo foi aberto corretamente
		if (!imagefile) {
			std::cerr << "Nao foi possivel abrir " << caminho << std::endl;
			return 1;
		}

		//sem comentarios no momento
		//imagefile << "# Arquivo " << caminho << std::endl;
		//imagefile << "# Resolucao: " << gerarResolucao(largura, altura) << std::endl;
		//imagefile << "# data: 16/08/2024\n" << std::endl;

		//eu preciso extrair os valores de linha e coluna da string resolucao! ajuda!!!! 
		// Ler os dados da imagem do arquivo .raw
		for (int i = 0; i < largura; i+=4) {

			int r, g, b, a;
			char separator;

			// Ler os valores RGB
			imagefile >> r >> separator >> g >> separator >> b >> separator >> a;

			dados[i] = static_cast<byte>(r);
			dados[i+1] = static_cast<byte>(g);
			dados[i+2] = static_cast<byte>(b);
			dados[i+3] = static_cast<byte>(a);

			// Ignorar espaço após cada pixel
			if (i < largura - 1) {
				imagefile.ignore(1, ' ');
			}

			// Fecha o arquivo
			imagefile.close();

			// Mensagem de sucesso
			std::cout << "Imagem foi carregada de " << caminho << std::endl;

		}
	}
	else {
		std::cout << "Formato nao implementado" << std::endl;
	}
	return 0;
}

std::string gerarResolucao(int l, int h) {
	return std::to_string(l) + "x" + std::to_string(h);
}

void Imagem::setNome(std::string nome_) {
	nome = nome_;
}

std::string Imagem::getNome() {
	return nome;
}

void Imagem::setExtensao(std::string extensao_) {
	nome = extensao_;
}

std::string Imagem::getExtensao() {
	return extensao;
}