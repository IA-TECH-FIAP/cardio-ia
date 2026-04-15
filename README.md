
# CardioIA — A Nova Era da Cardiologia Inteligente

## Integrantes do Grupo

| Nome | RM |
|------|-----|
| Giulia Bugatti Fonseca | 562675 |
| Mahmod Ahmad Issa | 561426 |
| Matheus Cardoso Oliveira Lima | 565844 |
| Silas Fernandes de Souza Fonseca | 564246 |

## Sobre o Projeto

O **CardioIA** é um projeto acadêmico desenvolvido no curso de Inteligência Artificial da **FIAP**, seguindo a metodologia **PBL (Project Based Learning)**. O objetivo é construir uma plataforma digital inteligente que simule o ecossistema de uma cardiologia moderna, integrando dados clínicos, modelos de Machine Learning, Visão Computacional, IoT e agentes inteligentes.

## Estrutura do Repositório

```
cardio-ia/
├── README.md                          # Este arquivo
├── LICENSE
├── .gitignore
├── fase-1-dados/                      # Fundação: Governança e Curadoria
│   ├── dataset/                       # heart.csv e links
│   └── docs/                          # Artigos Científicos 
│   └── imagens/                       # Imagens
└── fase-2-diagnostico/                # Implementação: IA e Desenvolvimento
    ├── parte 1/                # Parte 1: Extração de sintomas
    ├── parte 2/                # Parte 2: Machine Learning (TF-IDF)
    ├── ir-alem-1/             # Ir Além 1: Front-end React
    └── ir-alem-2/              # Ir Além 2: Rede Neural (Keras)
├── assets/                            # Artefatos da Fase 2 — Parte 1 (NLP + diagnóstico)
│   ├── mapa_conhecimento.csv          # Ontologia sintomas → doenças cardiovasculares
│   └── sintomas_pacientes.txt         # Frases de relatos de pacientes (linguagem natural)
├── src/                               # Código-fonte dos protótipos
│   └── diagnostico_cardiovascular.py  # Script de apoio ao diagnóstico (Fase 2 — Parte 1)
├── dataset/                           # Dataset utilizado
│   └── heart.csv
├── docs/                              # Textos médicos e imagens (Partes 2 e 3)
│   ├── *.pdf                          # 10 artigos científicos (NLP)
│   └── imagens/
│       ├── angiograma/
│       │   ├── labels.csv
│       │   ├── com_lesao/
│       │   │   ├── p1/
│       │   │   ├── p10/
│       │   │   ├── p11/
│       │   │   └── ...
│       │   └── sem_lesao/
│       │       ├── p1/
│       │       ├── p10/
│       │       ├── p16/
│       │       └── ...
│       ├── ecg/
│       │   └── train/
│       │       ├── batimento cardíaco anormal/
│       │       ├── histórico de infarto do miocárdio/
│       │       ├── infarto do miocárdio/
│       │       └── normal/
│       └── raio_x/
│           └── train/
│               ├── Fibrose/
│               ├── Hernia/
│               ├── Pleural/
│               └── Pneumonia/
```

## Governança de Dados e Viés

Desde o início do projeto, adotamos princípios de **Governança de Dados** e atenção ao **viés algorítmico**:

- **Origem e rastreabilidade**: toda fonte de dados é documentada com sua procedência, tipo de licença e limitações conhecidas.
- **Privacidade**: nenhum dado pessoal identificável de pacientes reais é utilizado. Quando os dados são provenientes de repositórios públicos, verificamos que já passaram por processos de anonimização.
- **Viés**: reconhecemos que datasets médicos podem conter vieses demográficos (sub-representação por gênero, etnia, faixa etária). Ao escolher e preparar os dados, buscamos identificar e documentar essas limitações para que sejam tratadas nas fases de modelagem.
- **Consentimento e ética**: dados de saúde são sensíveis. Utilizamos apenas fontes abertas e públicas, respeitando os termos de uso de cada repositório.

---

## 📂 Fase 1 — Batimentos de Dados (Fundação)

### Parte 1 — Dados Numéricos (IoT)

#### Descrição
Nesta etapa, organizamos um conjunto de dados estruturado contendo variáveis clínicas fundamentais para a predição de risco cardiovascular. O foco é fornecer uma base sólida que represente o estado hemodinâmico e metabólico do paciente para alimentar modelos de Machine Learning.

#### Link para os Dados
> **Dataset completo:** [Acesse via Google Drive](https://drive.google.com/file/d/1zXrqTZplxLK3EwAQ9lgl6Osovujn9AGA/view?usp=sharing)

#### Origem dos Dados
- **Fonte**: UCI Heart Disease Dataset - Cleveland Clinic Foundation.
- **Tipo**: Dados Reais.
- **Formato**: CSV (Comma-Separated Values).
- **Número de registros**: 303 instâncias.

#### Variáveis do Dataset

| Variável | Descrição | Relevância Clínica |
| --- | --- | --- |
| **Age** | Idade do paciente em anos. | Fator de risco fundamental; a probabilidade de doenças obstrutivas aumenta com a idade. |
| **Sex** | Sexo biológico (M: Masc, F: Fem). | Homens possuem risco precoce; mulheres apresentam perfis de risco distintos após a menopausa. |
| **ChestPainType** | Tipo de dor (TA, ATA, NAP, ASY). | A angina típica (TA) é o sinal subjetivo mais forte, elevando o risco de obstrução coronariana. |
| **RestingBP** | Pressão arterial em repouso (mm Hg). | Hipertensão causa estresse contínuo nas paredes arteriais, facilitando o rompimento de placas. |
| **Cholesterol** | Colesterol sérico (mg/dl). | Níveis elevados de LDL contribuem diretamente para a formação de placas ateroscleróticas. |
| **FastingBS** | Glicemia jejum (1: >120 mg/dl). | Diabetes acelera a aterosclerose por meio de inflamação vascular crônica. |
| **RestingECG** | Resultados do ECG em repouso. | Identifica sinais de hipertrofia ventricular esquerda ou anormalidades elétricas iniciais. |
| **MaxHR** | Freq. cardíaca máxima atingida. | Reflete a eficiência cronotrópica; falhas em atingir a meta indicam disfunção cardiovascular. |
| **ExerciseAngina** | Angina por exercício (Y: Sim, N: Não). | Marcador clássico de obstrução coronariana significativa sob estresse físico. |
| **Oldpeak** | Depressão de ST induzida por exercício. | Indicador eletrocardiográfico de isquemia miocárdica com alto valor preditivo para eventos agudos. |
| **ST_Slope** | Inclinação de ST (Up, Flat, Down). | Reflete a gravidade da isquemia; inclinações descendentes sugerem pior prognóstico. |
| **HeartDisease** | Presença de doença (1: Sim, 0: Não). | Variável alvo que permite o treinamento supervisionado de algoritmos de classificação. |

#### Justificativa para IA
Essas variáveis foram selecionadas por serem preditores biológicos validados em larga escala. Do ponto de vista da Inteligência Artificial, variáveis como `oldpeak` (ECG) e `ca` (fluoroscopia) possuem alta importância (*feature importance*) para algoritmos de Random Forest e Redes Neurais, permitindo que o modelo aprenda a correlação entre sintomas subjetivos (dor no peito) e marcadores fisiológicos objetivos (pressão e frequência cardíaca) para antecipar diagnósticos de infarto com acurácia superior aos escores de risco manuais.

---

### Parte 2 — Dados Textuais (NLP)

#### Descrição
Nesta etapa, reunimos artigos científicos (`.pdf`) relacionados a doenças cardíacas, saúde pública cardiovascular, sintomas ou tratamentos. Os textos estão armazenados na subpasta `docs/` deste repositório.

#### Textos Coletados
- 10 Artigos Científicos (SciELO / BVS) abrangendo temas como: Infarto Agudo do Miocárdio, Miocardiopatia, Estresse Percebido e Hipertensão na Gestação.

#### Aplicações em NLP
| Técnica de NLP | Descrição | Relevância para o CardioIA |
|----------------|-----------|---------------------------|
| **Extração de entidades (NER)** | Identificação automática de sintomas, medicamentos, diagnósticos. | Constrói base de conhecimento estruturada. |
| **Classificação de tópicos** | Categorização automática em temas de saúde. | Organização de grandes volumes de literatura médica. |
| **Análise de sentimentos** | Avaliação do tom e gravidade de relatos clínicos. | Priorização de atendimentos por urgência. |
| **Sumarização automática** | Geração de resumos de artigos extensos. | Otimização do tempo clínico dos profissionais. |
| **Extração de relações** | Identificação de causas e efeitos (ex: Hipertensão -> IC). | Mapeia grafos para raciocínio clínico automatizado. |

---

### Parte 3 — Dados Visuais (Visão Computacional)

#### Descrição
Reunimos imagens médicas de três exames cardiológicos fundamentais: ECG, Raios-X torácicos e Angiogramas coronarianos, organizados por tipo e classe clínica.

#### Resumo das Fontes Utilizadas
| Tipo de exame | Base escolhida | Quantidade |
|---------------|----------------|------------|
| ECG | ECG Images dataset of Cardiac Patients (Mendeley Data) | 928 imagens |
| Raio-X torácico | NIH Chest X ray-14 (Kaggle) | 75.168 imagens |
| Angiogramas | CADICA (Mendeley Data) | 6.126 imagens |

#### Aplicações em Visão Computacional
| Técnica de VC | Descrição | Relevância para o CardioIA |
|---------------|-----------|---------------------------|
| **Detecção de padrões** | CNNs reconhecendo ritmos ou calcificações. | Triagem automatizada de exames. |
| **Segmentação** | Delimitar estruturas anatômicas (contornos cardíacos). | Medições automatizadas (ex: tamanho do coração). |
| **Reconhecimento de anomalias** | Classificação em "normal" ou "anormal". | Segunda opinião digital para o cardiologista. |

---

## 🧠 Fase 2 — Diagnóstico Automatizado (Implementação)

Nesta fase, o CardioIA evolui para a **automatização do diagnóstico**, utilizando os dados processados na fase anterior para criar módulos inteligentes de análise e triagem.

### 🎥 Demonstrações em Vídeo
- 🎥 **Parte 2 (NLP & Machine Learning):** [Link do YouTube]
- 🎥 **Ir Além 1 (Portal React + Vite):** [Link do YouTube]
- 🎥 **Ir Além 2 (Rede Neural Keras):** [Link do YouTube]

### Parte 1 — Extração de Sintomas e Ontologia
Implementamos um sistema capaz de interpretar relatos médicos em texto e sugerir diagnósticos assistidos por IA.
- **Relatos:** 10 frases completas simulando descrições de sintomas de pacientes.
- **Mapa de Conhecimento:** Estrutura `.csv` associando sintomas (ex: "aperto no tórax") a possíveis diagnósticos (ex: Infarto).
- **Código Python:** Leitura automatizada e cruzamento de informações para sugestão de diagnóstico.

### Parte 2 — Classificador de Risco (Machine Learning)
Desenvolvemos um classificador básico de texto para triagem clínica, categorizando relatos entre **Baixo Risco** e **Alto Risco**.
- **Processamento:** Uso de **TF-IDF** para vetorização numérica das frases.
- **Modelo:** Comparação entre modelos, com destaque para a **Regressão Logística**, que obteve **95% de acurácia** devido à estabilidade em datasets menores (100 linhas).
- **Avaliação:** Uso de matriz de confusão e análise de pesos das palavras para garantir transparência nas predições de alto risco.

### Ir Além 1 — Portal CardioIA (React + Vite)
Interface responsiva simulando um portal hospitalar para visualização de dados e métricas de pacientes.
- **Tecnologias:** React, Context API para autenticação, proteção de rotas e listagem via API simulada.

### Ir Alem 2 -- Diagnostico Visual em Cardiologia com Rede Neural

#### Sobre a Atividade

Esta atividade aplica uma **Rede Neural Artificial do tipo MLP (Perceptron Multicamadas)** para classificar imagens de eletrocardiogramas (ECG) em duas categorias: **normal** e **anormal**. O objetivo e demonstrar como a Inteligencia Artificial pode auxiliar na triagem automatizada de pacientes cardiologicos por meio de diagnostico visual.

---

#### Dataset Utilizado

- **Fonte:** ECG Images dataset of Cardiac Patients -- Mendeley Data
- **Link:** https://data.mendeley.com/datasets/gwbz3fsgp8/2
- **Licenca:** CC BY 4.0
- **Total:** 928 imagens JPG (2213x1572 pixels, coloridas)

#### Classes Originais

| Classe | Quantidade |
|--------|------------|
| Normal | 284 |
| Batimento cardiaco anormal | 233 |
| Infarto do miocardio | 239 |
| Historico de infarto do miocardio | 172 |

#### Agrupamento Binario

Para a classificacao binaria exigida pela atividade, as classes foram reorganizadas:

| Classe | Label | Quantidade | Composicao |
|--------|-------|------------|------------|
| Normal | 0 | 284 | Classe "normal" original |
| Anormal | 1 | 644 | Batimento anormal + Infarto + Historico de infarto |

![Distribuicao das Classes](/fase-2-diagnostico/ir-alem-2/imagens/distribuicao_classes.png)

---

#### Estrutura de Pastas

```
cardio-ia-fase-2-parte-1/
├── docs/
│   └── imagens/
│       └── ecg/
│           └── train/
│               ├── normal/                              # 284 imagens
│               ├── batimento cardiaco anormal/           # 233 imagens
│               ├── infarto do miocardio/                 # 239 imagens
│               └── historico de infarto do miocardio/    # 172 imagens
│
├── ir_alem_2/
│   ├── IR_ALEM_2.md                                     # Este arquivo
│   ├── ecg_diagnostico_visual.ipynb                     # Notebook com o codigo completo
│   └── imagens/                                         # Graficos e resultados visuais
│       ├── distribuicao_classes.png
│       ├── exemplos_ecg.png
│       ├── preprocessamento.png
│       ├── matriz_confusao.png
│       └── curvas_aprendizado.png
│
└── README.md
```

---

#### Metodologia

#### 1. Exemplos de Imagens do Dataset

Abaixo, exemplos reais de ECGs do dataset utilizado. A linha superior mostra ECGs normais e a inferior mostra ECGs anormais:

![Exemplos de ECG Normal e Anormal](/fase-2-diagnostico/ir-alem-2/imagens/exemplos_ecg.png)

#### 2. Pre-processamento das Imagens

As imagens originais (2213x1572, RGB) passam por quatro transformacoes antes de alimentar a rede neural:

| Etapa | Transformacao | Justificativa |
|-------|---------------|---------------|
| Grayscale | 3 canais (RGB) para 1 canal | A informacao diagnostica do ECG esta na forma da onda, nao nas cores. Reduz features em 3x |
| Resize | 64x64 pixels | Reduz dimensionalidade para 4.096 pixels. Tamanho escolhido para manter uma relacao saudavel entre features e amostras |
| Flatten | Matriz 64x64 para vetor de 4.096 | MLP exige entrada unidimensional (diferente de CNNs que aceitam matrizes) |
| Normalizacao | Pixels de [0, 255] para [0, 1] | Estabiliza o treinamento e acelera a convergencia do otimizador |

O resultado visual do pre-processamento:

![Pre-processamento: Original vs Processada](/fase-2-diagnostico/ir-alem-2/imagens/preprocessamento.png)

#### 3. Divisao dos Dados

- **80% para treino** (742 imagens)
- **20% para teste** (186 imagens)
- Divisao **estratificada** para manter a proporcao normal/anormal em ambos os conjuntos

#### 4. Tratamento do Desbalanceamento

O dataset possui ~30% Normal vs ~70% Anormal. Sem tratamento, o modelo tende a classificar tudo como a classe majoritaria (Anormal). Para resolver isso, utilizamos **class_weight** com fator 3.4x mais peso na classe Normal, forcando o modelo a penalizar mais os erros na classe minoritaria.

#### 5. Arquitetura da Rede Neural (MLP)

```
Camada de Entrada: 4.096 neuronios (imagem achatada)
        |
Dense(256, ativacao='relu')
BatchNormalization()
Dropout(0.3)
        |
Dense(128, ativacao='relu')
BatchNormalization()
Dropout(0.3)
        |
Dense(64, ativacao='relu')
Dropout(0.2)
        |
Dense(1, ativacao='sigmoid')  -->  Saida: probabilidade [0, 1]
```

**Resumo do modelo (Keras):**

| Camada | Output Shape | Parametros |
|--------|-------------|------------|
| Dense (256, ReLU) | (None, 256) | 1.048.832 |
| BatchNormalization | (None, 256) | 1.024 |
| Dropout (0.3) | (None, 256) | 0 |
| Dense (128, ReLU) | (None, 128) | 32.896 |
| BatchNormalization | (None, 128) | 512 |
| Dropout (0.3) | (None, 128) | 0 |
| Dense (64, ReLU) | (None, 64) | 8.256 |
| Dropout (0.2) | (None, 64) | 0 |
| Dense (1, Sigmoid) | (None, 1) | 65 |
| **Total** | | **1.091.585** |

**Decisoes de arquitetura:**

- **ReLU (Rectified Linear Unit):** funcao de ativacao que resolve o problema do vanishing gradient e permite treinamento mais rapido que sigmoid/tanh nas camadas ocultas
- **BatchNormalization:** normaliza as ativacoes entre camadas, estabilizando e acelerando o treinamento
- **Dropout (20-30%):** tecnica de regularizacao que "desliga" neuronios aleatoriamente durante o treino -- essencial com dataset pequeno (928 imagens) para evitar overfitting
- **Sigmoid na saida:** retorna um valor entre 0 e 1, interpretado como probabilidade de ser "anormal"
- **Rede compacta (256-128-64):** arquitetura dimensionada para o tamanho do dataset, evitando excesso de parametros

#### 6. Treinamento

| Parametro | Valor | Justificativa |
|-----------|-------|---------------|
| Otimizador | Adam (lr=0.0003) | Taxa de aprendizado reduzida para convergencia mais estavel com dataset pequeno |
| Funcao de perda | Binary Crossentropy | Padrao para classificacao binaria |
| Metrica | Accuracy | Exigida pelo enunciado |
| Epochs | 150 (max) | Limite superior, controlado pelo EarlyStopping |
| Batch size | 16 | Batches menores para melhor generalizacao com poucas amostras |
| EarlyStopping | patience=15 | Para o treinamento se a val_loss nao melhorar por 15 epochs consecutivas |
| Class weight | Normal: 2.45, Anormal: 0.72 | Compensa o desbalanceamento entre as classes |

O treinamento finalizou automaticamente na **epoch 44** (de 150 possiveis), pois o EarlyStopping detectou que a val_loss parou de melhorar.

---

#### Resultados Obtidos

#### Acuracia

| Metrica | Valor |
|---------|-------|
| **Acuracia geral** | **86.02%** |
| Loss no teste | 0.3555 |

#### Relatorio de Classificacao

| Classe | Precision | Recall | F1-Score | Amostras |
|--------|-----------|--------|----------|----------|
| Normal | 0.86 | 0.65 | 0.74 | 57 |
| Anormal | 0.86 | 0.95 | 0.90 | 129 |
| **Macro avg** | **0.86** | **0.80** | **0.82** | **186** |

O modelo identifica **95% dos casos anormais** (alto recall), o que e desejavel em contexto medico -- minimiza o risco de liberar pacientes com condicoes cardiacas nao detectadas.

#### Matriz de Confusao

![Matriz de Confusao](/fase-2-diagnostico/ir-alem-2/imagens/matriz_confusao.png)

#### Curvas de Aprendizado

Os graficos abaixo mostram a evolucao da acuracia e da loss ao longo das epochs de treinamento. A comparacao entre treino e validacao permite identificar se houve overfitting.

![Curvas de Aprendizado - Acuracia e Loss](/fase-2-diagnostico/ir-alem-2/imagens/curvas_aprendizado.png)

---

#### Tecnologias Utilizadas

| Biblioteca | Versao | Uso |
|------------|--------|-----|
| TensorFlow / Keras | 2.x | Construcao, compilacao e treinamento da MLP |
| OpenCV (cv2) | 4.x | Carregamento, redimensionamento e conversao de imagens |
| scikit-learn | 1.x | Split treino/teste, class_weight, metricas de avaliacao |
| NumPy | 2.x | Manipulacao de arrays e vetores |
| Matplotlib / Seaborn | - | Graficos e visualizacoes |

---

#### Limitacoes e Proximos Passos

- **MLP vs CNN**: a MLP trata cada pixel como feature independente, perdendo informacoes espaciais. Uma CNN seria mais adequada para visao computacional, pois preserva relacoes entre pixels vizinhos.
- **Dataset pequeno**: 928 imagens e um volume reduzido para deep learning. Data augmentation poderia expandir artificialmente o dataset.
- **Desbalanceamento**: utilizamos class_weight para mitigar o vies para a classe majoritaria. Oversampling (SMOTE) poderia complementar.

---

#### Como Executar

1. Certifique-se de ter Python 3.8+ instalado
2. Instale as dependencias:
   ```bash
   pip install tensorflow opencv-python scikit-learn matplotlib seaborn numpy
   ```
3. Abra o notebook:
   ```bash
   jupyter notebook ir_alem_2/ecg_diagnostico_visual.ipynb
   ```
4. Execute todas as celulas sequencialmente

---

## Referências
1. UCI Machine Learning Repository: Heart Disease Dataset.
2. Mendeley Data: ECG Images e CADICA Dataset.
3. Kaggle: NIH Chest X ray-14.
4. SciELO & BVS: Literatura Médica Brasileira.
5. ECG Images dataset of Cardiac Patients
6. Keras Documentation: https://keras.io
7. Scikit-learn Documentation: https://scikit-learn.org

---

## Licença
Este projeto é de caráter acadêmico, desenvolvido para fins educacionais no curso de Inteligência Artificial da FIAP.
