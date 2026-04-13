# 🕹️ Mario SNES Prototype (PVSnesLib)

## 📌 Sobre o projeto

Este projeto é um protótipo de jogo para Super Nintendo (SNES) desenvolvido utilizando a **PVSnesLib**, um framework que permite escrever código em C que é posteriormente compilado para Assembly direcionado ao processador **Ricoh 5A22** do SNES, baseado no processador **65c816**.

A base inicial do projeto foi o exemplo “vanilla” fornecido pela própria PVSnesLib — uma versão extremamente simples, com gráficos básicos (em preto e branco) e sem implementações mais sofisticadas de lógica de jogo.

A partir dessa base, o projeto foi evoluído com o objetivo de estudar e compreender aspectos fundamentais do desenvolvimento para o SNES, incluindo renderização, organização de gráficos na ROM e comportamento de gameplay.

> ⚠️ Este projeto é focado em aprendizado de arquitetura SNES e baixo nível.

---

## 🎯 Objetivo

O principal objetivo deste projeto é servir como:

*  Estudo prático de desenvolvimento para SNES
*  Entendimento da arquitetura gráfica e de memória do console
*  Experimentação com otimizações como **HiROM** e **FastROM**
*  Organização de assets (sprites, tiles, mapas) dentro da ROM

---

## 🎥 Preview

### Antes vs Depois
<img width="1280" height="435" alt="Image" src="https://github.com/user-attachments/assets/9d1b711c-0ed9-487d-a752-787f9b63047b" />

### Construção da fase (Tiled)
![Alt text](https://github.com/andykallian/homebrewMarioLike/blob/542bd488f53659d7d78b703e8498821d154bea27/1.png)

### Gráficos e paleta (Gale)
![Alt text]<img width="1107" height="752" alt="Image" src="https://github.com/user-attachments/assets/e68c30bf-7639-4e05-8c98-e2fe2db2331c" />

### Configuração da ROM (HiROM + FastROM)
<img width="1197" height="499" alt="Image" src="https://github.com/user-attachments/assets/ab1dcf7a-aed9-4313-9f5d-c47434739fc3" />

---

## 🔧 O que foi implementado

Partindo de uma base extremamente simples, foram adicionados diversos sistemas essenciais para um jogo funcional:

###  Sistema de movimentação

* Controle do personagem via input
* Movimentação em múltiplas direções

###  Colisão com tiles

* Implementação de lógica de colisão baseada no mapa
* Bloqueio de movimento em áreas não caminháveis

###  Animação de sprites

* Sistema de troca de frames (poses)
* Animação baseada no estado do personagem (parado / andando)
* Organização de sprites em sequência contínua na ROM

###  Renderização de mapa

* Uso de **background (tilemap)** para representar o cenário
* Separação entre:

  * mapa (BG)
  * personagem (sprite/OBJ)

###  Organização de gráficos na ROM

* Estruturação de sprites em blocos contínuos
* Controle manual de offset para acesso aos dados
* Preparação para uso de seções (`sections`) no linker

###  Compilação em HiROM + FastROM

| Tipo    | Descrição                                                          |
| ------- | ------------------------------------------------------------------ |
| HiROM   | Melhor organização de memória e acesso direto a grandes volumes    |
| FastROM | Acesso mais rápido (~120ns vs ~200ns), melhorando leitura de dados |

---

## ▶ Como executar

Para rodar o projeto:

1. Baixe o arquivo **`mariolike.sfc`** disponível no repositório
2. Abra em qualquer emulador de SNES (ex: Snes9x, mesen, bsnes, etc.)
3. Alternativamente, você pode rodar em hardware real utilizando flashcart compatível

>  Não é necessário compilar o projeto para testar — basta executar a ROM.

##  Arquitetura básica

```
Background (mapa)
        +
Sprites (personagem)
        =
Renderização final
```

* O mapa é um **tilemap (BG)**
* O personagem é um **OBJ (sprite)**
* O movimento percebido é uma combinação de:

  * deslocamento do sprite
  * scroll do background

---

##  Aprendizados principais

Durante o desenvolvimento, alguns conceitos fundamentais ficaram claros:

* O SNES não “entende” sprites como entidades — tudo é manipulação de memória
* Organização na ROM é responsabilidade do desenvolvedor (via linker)
* Performance depende fortemente de como os dados são organizados
* Separar lógica (C) de layout de memória (linkfile) é essencial
* O uso correto de DMA é crítico para gráficos

---

##  Próximos passos

Possíveis evoluções do projeto:

* Sistema de câmera com scroll contínuo
* Mapa maior com streaming de dados
* Sistema de múltiplos personagens
* NPCs com comportamento
* Transição de uma fase para o mundo aberto e vice-versa
* Transição entre os modos de background do SNES

---

##  Referências

* Documentação da PVSnesLib
* Arquitetura do SNES (Ricoh 5A22 / 65c816)
* SNESLAB
* SFC Development Wiki

---

## 🏁 Conclusão

Este projeto representa uma evolução significativa a partir de um exemplo básico, demonstrando na prática como construir sistemas fundamentais de um jogo no SNES.

Mais do que um protótipo jogável, ele serve como uma base sólida para o entendimento de conceitos mais complexos e como registro do processo de aprendizado no desenvolvimento para hardware retrô.