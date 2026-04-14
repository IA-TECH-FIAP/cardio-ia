
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
│   └── docs/                          # Artigos Científicos e imagens
└── fase-2-diagnostico/                # Implementação: IA e Desenvolvimento
    ├── 01-diagnostico-nlp/            # Parte 1: Extração de sintomas
    ├── 02-classificador-risco/        # Parte 2: Machine Learning (TF-IDF)
    ├── 03-ir-alem-portal/             # Ir Além 1: Front-end React
    └── 04-ir-alem-visao/              # Ir Além 2: Rede Neural (Keras)
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
- 🎥 **Parte 1 e 2 (NLP & Machine Learning):** [Link do YouTube]
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

### Ir Além 2 — Diagnóstico Visual (Rede Neural MLP)
Aplicação de Rede Neural Artificial (Keras/TensorFlow) para classificação de imagens de eletrocardiograma.
- **Dataset:** Heartbeat Dataset (Kaggle).
- **Objetivo:** Diferenciar ritmos cardíacos normais de anomalias de forma automatizada.

---

## Referências
1. UCI Machine Learning Repository: Heart Disease Dataset.
2. Mendeley Data: ECG Images e CADICA Dataset.
3. Kaggle: NIH Chest X ray-14.
4. SciELO & BVS: Literatura Médica Brasileira.

---

## Licença
Este projeto é de caráter acadêmico, desenvolvido para fins educacionais no curso de Inteligência Artificial da FIAP.