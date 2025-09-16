# 🏃‍♀️ Projeto IoT – Monitoramento de Saúde para Jogadoras de Futebol de Base

Este projeto utiliza Internet das Coisas (IoT) para monitorar em tempo real a saúde das jogadoras, automatizar o registro de presença e disponibilizar os dados em um painel online para análise de desempenho e prevenção de lesões.

---

## 📌 Sumário

- [Visão Geral](#visão-geral)  
- [Conceitos de IoT](#conceitos-de-iot)  
- [Arquitetura do Sistema](#arquitetura-do-sistema)  
- [Tecnologias Utilizadas](#tecnologias-utilizadas)  
- [Desenvolvimento](#desenvolvimento)  
- [Demonstração](#demonstração)  
- [Benefícios](#benefícios)  

---

## 📖 Visão Geral

A solução foi desenvolvida para monitorar a saúde das jogadoras de futebol de base, utilizando uma pulseira inteligente que:

- ✅ Faz o registro de presença ao aproximar do totem  
- ✅ Coleta dados de saúde durante treinos e partidas (batimentos cardíacos, movimentação, tempo ativo)  
- ✅ Envia os dados para a nuvem via protocolo MQTT em tempo real  
- ✅ Salva o histórico para análise futura  
- ✅ Exibe os resultados em um dashboard online, auxiliando técnicos e profissionais de saúde na prevenção de lesões e na melhora do desempenho das atletas  

---

## 🌐 Conceitos de IoT

A Internet das Coisas (IoT) conecta objetos do mundo físico à internet, permitindo que eles coletem, transmitam e processem dados automaticamente.

No contexto deste projeto, a IoT é fundamental para:

- Automatizar o fluxo de informações sem intervenção manual  
- Garantir coleta contínua e em tempo real  
- Facilitar a análise inteligente de dados, permitindo decisões baseadas em evidências  
- Melhorar o desempenho esportivo e reduzir riscos de lesões  

---

## 🏗 Arquitetura do Sistema


<img width="1024" height="1024" alt="diagrama backend" src="https://github.com/user-attachments/assets/526fc850-e0fc-43a1-9e04-0c3e7ba139b0" />

    
### Fluxo de dados

- A jogadora aproxima a pulseira do totem → registro de presença é enviado  
- Durante o jogo, a pulseira envia dados de saúde via MQTT  
- O IoT Agent traduz as mensagens para NGSIv2  
- O Orion Context Broker gerencia os dados de contexto em tempo real  
- O STH-Comet armazena os dados históricos no MongoDB  
- O dashboard web exibe relatórios e gráficos de desempenho  

---

### 🛠 Tecnologias Utilizadas

- **FIWARE** – Plataforma open-source para gerenciamento de contexto  
- **MQTT (HiveMQ)** – Protocolo de comunicação leve para IoT  
- **STH-Comet + MongoDB** – Armazenamento de histórico  
- **Docker Compose** – Orquestração dos containers  
- **Arduino/Wokwi** – Simulação e prototipagem do dispositivo  

---

### 👨‍💻 Desenvolvimento

#### Configuração da Plataforma IoT
- Deploy dos containers do FIWARE (Orion, IoT Agent, MongoDB, STH-Comet) com Docker Compose  
- Configuração do HiveMQ como broker MQTT  

#### Criação de Dispositivos Virtuais
- Configuração do IoT Agent com API Key e Device ID  
- Simulação do envio de dados usando Wokwi  

#### Protocolos de Comunicação
- Utilização do MQTT para telemetria  
- HTTP/NGSIv2 para atualização de entidades no Orion  

---

### 🎥 Demonstração

1. Jogadora aproxima a pulseira do totem → login no sistema  
2. Durante o jogo, dados são enviados em tempo real para o Orion  
3. Após o jogo, pulseira é aproximada novamente → sessão é encerrada e dados são salvos  
4. Dashboard exibe gráficos de desempenho e indicadores de saúde
    
<img width="1161" height="822" alt="image" src="https://github.com/user-attachments/assets/42aa034d-5432-4cbb-9da6-41fd527ce155" />

Simulação do Totem (com botões, pois o Wokwi não identifica touch):

- Acesse o link: 👉 [Simulação no Wokwi](https://wokwi.com/projects/442099339610562561)

para ver os dados da pulseira sendo salvos acesse:

- 🎥 [Clique aqui para assistir no YouTube](https://youtu.be/LEI4tAASEGI)



---

### 🎯 Benefícios

- **Automação:** Sem necessidade de digitação ou papelada  
- **Monitoramento Contínuo:** Dados em tempo real  
- **Histórico Centralizado:** Facilita análise de longo prazo  
- **Prevenção de Lesões:** Identifica riscos antes de se tornarem problemas  
- **Decisão Baseada em Dados:** Melhora o planejamento técnico

## 👨‍💻 Autoras

Ana Luiza De Franco e Rinaldi - RM:564061 

Giovana Gaspar Larocca - RM:564965 

Giovanna Lins Sayama - RM:565901 

Rayssa Luzia Portela Aquino - RM:562024

📍 São Paulo - Brasil

---
