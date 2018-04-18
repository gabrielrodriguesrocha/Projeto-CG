# Computação Gráfica

Membros da equipe:

1. [Breno Vinicius Viana de Oliveira](https://github.com/donOnerb) - RA 726498
2. [Gabriel Rodrigues Rocha](https://github.com/gabrielrodriguesrocha) - RA 726518
3. [Henrique Shinki Kodama](https://github.com/hskodama) - RA 726537

A parte de OpenGL deste projeto é baseada (ou descaradamente copiada) do repositório [Glitter](https://github.com/Polytonic/Glitter). Os arquivos referentes a essa parte encontram-se no diretório [Dist](https://github.com/gabrielrodriguesrocha/Projeto-CG/tree/master/Dist).

A parte de WebGL deste projeto é baseada (ou descaradamente copiada) dos tutoriais do blog [Bits in Pieces](https://dannywoodz.wordpress.com/). Os arquivos referentes a essa parte encontram-se no diretório [Web](https://github.com/gabrielrodriguesrocha/Projeto-CG/tree/master/Web).

# Reconhecimentos

- Kevin Fung, pelo Glitter;
- Danny Woodz (?), pelo código em WebGL.

# Compilação local

Instaladas as [dependências](https://github.com/gabrielrodriguesrocha/Projeto-CG/wiki/Dependencias), basta executar `make` na pasta raiz do projeto e o executável será gerado na pasta `Build`.

# Compilação _standalone_

As instruções de compilação são idênticas àquelas descritas no repositório [Glitter](https://github.com/Polytonic/Glitter). Se o repositório foi clonado sem a flag `--recursive`, é necessário obter o submódulos manualmente, por meio do comando `git submodule update --recursive --remote`.

## Linux

Para compilar o projeto, é necessário executar os seguintes comandos:
```
cd Build
cmake ..
make -jN
```
Onde `N` é o número de núcleos do processador onde a compilação acontecerá.

## macOS

A compilação neste caso requer o [Xcode](https://developer.apple.com/xcode/) e possui instruções semelhantes:
```
cd Build
cmake -G "Xcode" ..
make -jN
```
## Windows

A compilação neste caso requer o [Visual Studio Community](https://www.visualstudio.com/pt-br/vs/community/) com as extensões de VC++:
```
cd Build
cmake -G "Visual Studio 15" ..
cmake -G "Visual Studio 15 Win64" ..
```
O que será gerado pelo `cmake` não é um Makefile mas sim uma solução do Visual Studio. Para compilar, é necessário abrir o arquivo `Dist.sln` gerado e compilar o projeto.

## Compilação _web_

É JavaScript! Basta abrir o arquivo `index.html`.
