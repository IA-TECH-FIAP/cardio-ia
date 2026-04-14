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

Este repositório corresponde à **Fase 1 — Batimentos de Dados**, na qual assumimos o papel de cientistas de dados hospitalares. O desafio central é **levantar, organizar e compreender dados cardiológicos** que, nas fases seguintes, alimentarão os módulos inteligentes da plataforma.

## Estrutura do Repositório

```
cardio-ia/
├── README.md                          # Este arquivo
├── LICENSE
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

## Parte 1 — Dados Numéricos (IoT)

### Descrição

Nesta etapa, organizamos um conjunto de dados estruturado contendo variáveis clínicas fundamentais para a predição de risco cardiovascular. O foco é fornecer uma base sólida que represente o estado hemodinâmico e metabólico do paciente para alimentar modelos de Machine Learning.

### Link para os Dados

> **Dataset completo:**([https://drive.google.com/file/d/1zXrqTZplxLK3EwAQ9lgl6Osovujn9AGA/view?usp=sharing](https://drive.google.com/file/d/1zXrqTZplxLK3EwAQ9lgl6Osovujn9AGA/view?usp=sharing))

### Origem dos Dados

- **Origem dos Dados**
- **Fonte: UCI Heart Disease Dataset - Cleveland Clinic Foundation**.
- **Tipo: Dados Reais**.
- **Formato: CSV (Comma-Separated Values).**
- **Número de registros: 303 instâncias.**

### Variáveis do Dataset

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

### Justificativa para IA

Essas variáveis foram selecionadas por serem preditores biológicos validados em larga escala. Do ponto de vista da Inteligência Artificial, variáveis como `oldpeak` (ECG) e `ca` (fluoroscopia) possuem alta importância (*feature importance*) para algoritmos de Random Forest e Redes Neurais, permitindo que o modelo aprenda a correlação entre sintomas subjetivos (dor no peito) e marcadores fisiológicos objetivos (pressão e frequência cardíaca) para antecipar diagnósticos de infarto com acurácia superior aos escores de risco manuais.

---

## Parte 2 — Dados Textuais (NLP)

### Descrição

Nesta etapa, reunimos artigos científicos (`.pdf`) relacionados a doenças cardíacas, saúde pública cardiovascular, sintomas ou tratamentos. Os textos estão armazenados na subpasta `docs/` deste repositório.

### Textos Coletados

| Arquivo | Título / Descrição | Fonte |
|---------|-------------------|-------|
| `docs/incidencia_e_fatores_de_risco_associados_ao_infarto_agudo_do_miocardio_sem_obstrucao_coronariana.pdf` | Incidência e fatores de risco associados ao infarto agudo do miocárdio sem obstrução coronariana | SciELO |
| `docs/miocardiopatia_nao_compactada_uma_visao_atual.pdf` | Miocardiopatia não compactada: uma visão atual | SciELO |
| `docs/fatores_de_risco_cardiovascular_saberes_e_praticas_de_cuidado_de_mulheres_possibilidade_para_rever_habitos.pdf` | Fatores de risco cardiovascular: saberes e práticas de cuidado de mulheres — possibilidade para rever hábitos | BVS |
| `docs/anti_inflamatorios_nao_esteroides_efeitos_cardiovasculares_cerebro_vasculares_e_renais.pdf` | Anti-inflamatórios não esteroides: efeitos cardiovasculares, cerebrovasculares e renais | SciELO |
| `docs/prevalencia_e_caracterizacao_do_fenotipo_de_cardiomiopatia_hipertrofica_avaliado_por_ecocardiografia_em_uma_populacao_adulta_brasileira_o_estudo_elsa_brasil.pdf` | Prevalência e caracterização do fenótipo de cardiomiopatia hipertrófica avaliado por ecocardiografia em uma população adulta brasileira: o estudo ELSA-Brasil | SciELO |
| `docs/hipertensao_arterial_sistemica_como_fator_determinante_para_desencadeamento_de_hemorragias_encefalicas_uma_analise_reflexiva.pdf` | Hipertensão arterial sistêmica como fator determinante para desencadeamento de hemorragias encefálicas: uma análise reflexiva | BVS |
| `docs/associacao_do_estresse_percebido_e_qualidade_do_sono_de_hipertensos_resistentes_estudo_transversal.pdf` | Associação do estresse percebido e qualidade do sono de hipertensos resistentes: estudo transversal | SciELO |
| `docs/fatores_associados_ao_risco_cardiovascular_em_pessoas_vivendo_com_hiv_aids.pdf` | Fatores associados ao risco cardiovascular em pessoas vivendo com HIV/AIDS | SciELO |
| `docs/hipertensao_arterial_na_gestacao.pdf` | Hipertensão arterial na gestação | BVS |
| `docs/doencas_cardiovasculares_em_trabalhadores_da_saude_fatores_de_risco_e_estrategias_preventivas_no_brasil.pdf` | Doenças cardiovasculares em trabalhadores da saúde: fatores de risco e estratégias preventivas no Brasil | BVS |

### Fontes

- **SciELO** (Scientific Electronic Library Online): https://scielo.org
- **BVS** (Biblioteca Virtual em Saúde): https://bvsalud.org

### Aplicações em NLP

Os textos coletados poderão ser explorados por algoritmos de Processamento de Linguagem Natural das seguintes formas:

| Técnica de NLP | Descrição | Relevância para o CardioIA |
|----------------|-----------|---------------------------|
| **Extração de entidades (NER)** | Identificação automática de sintomas, medicamentos, diagnósticos e procedimentos mencionados nos textos. | Permite construir uma base de conhecimento estruturada a partir de textos médicos não estruturados, alimentando sistemas de apoio à decisão clínica. |
| **Classificação de tópicos** | Categorização automática dos textos em temas como "fatores de risco", "tratamentos", "prevenção" e "diagnóstico". | Facilita a organização de grandes volumes de literatura médica, permitindo que o sistema direcione informações relevantes a cada módulo do CardioIA. |
| **Análise de sentimentos** | Avaliação do tom e da gravidade das informações presentes em relatos clínicos ou literatura médica. | Pode ser aplicada a relatos de pacientes ou notas clínicas para identificar urgência ou severidade de quadros, priorizando atendimentos. |
| **Sumarização automática** | Geração de resumos concisos a partir de artigos extensos. | Auxilia profissionais de saúde a obter rapidamente as informações mais relevantes de publicações científicas, otimizando o tempo clínico. |
| **Extração de relações** | Identificação de relações entre entidades (ex.: "hipertensão *causa* insuficiência cardíaca"). | Permite mapear grafos de conhecimento médico que podem ser usados por agentes inteligentes para raciocínio clínico automatizado. |

### Justificativa

A análise de textos médicos via NLP é fundamental para o CardioIA porque grande parte do conhecimento clínico está registrada em formato textual — artigos, prontuários, diretrizes e relatos. Algoritmos de NLP permitem transformar esse conhecimento não estruturado em dados acionáveis, viabilizando funcionalidades como triagem automatizada baseada em sintomas relatados, assistentes virtuais de saúde e sistemas de alerta baseados em literatura atualizada.

---

## Parte 3 — Dados Visuais (Visão Computacional)

### Descrição

Nesta etapa, reunimos imagens médicas de três tipos de exames cardiológicos, com foco em aplicações de Visão Computacional no projeto CardioIA:

- Eletrocardiogramas (ECG)
- Raios-X torácicos
- Angiogramas coronarianos

A proposta foi selecionar bases que realmente contêm imagens (.png, .jpg ou equivalentes), com acesso relativamente simples para uso acadêmico, e estruturar os arquivos em pastas organizadas por tipo de exame e por classe clínica sempre que possível.

### Tipos de Exame Selecionados

#### 1. Eletrocardiogramas (ECG)

Tipo de exame: Eletrocardiogramas (ECG)  
Número de imagens: 928 imagens  
Formato: PNG / imagens de ECG  
Fonte: Mendeley Data — ECG Images dataset of Cardiac Patients  
Link da fonte: https://data.mendeley.com/datasets/gwbz3fsgp8/2  
Licença: CC BY 4.0

Para a modalidade de ECG, foi selecionada a base ECG Images dataset of Cardiac Patients, disponibilizada no Mendeley Data. Trabalhos recentes que utilizam essa base descrevem um total de 928 imagens de ECG, organizadas em quatro classes clínicas: condição normal, infarto do miocárdio, batimento anormal e histórico de infarto.

Arquitetura de pastas (ECG):
```
imagens/
└── ecg/
    ├── normal/
    ├── batimento_anormal/
    ├── infarto/
    └── historico_infarto/
```
#### 2. Raios-X torácicos

Tipo de exame: Raio-X torácico  
Número de imagens: 75.168 imagens consideradas nesta seleção  
Formato: PNG / JPG  
Fonte: Kaggle — NIH Chest X ray-14 (224 x 224, disease wise folder)  
Link da fonte: https://www.kaggle.com/datasets/khanfashee/nih224folderwise-data  
Licença: CC0: Public Domain

Para a modalidade de raio-X torácico, foi escolhida a versão do dataset NIH Chest X-ray organizada em pastas por doença no Kaggle. Nesta etapa, foram selecionadas especificamente as classes No Finding, Cardiomegaly, Fibrosis, Hernia, Pleural Thickening, Pneumonia e Pneumothorax, totalizando 75.168 arquivos somados nessas pastas. A base original NIH informa 15 classes no total, sendo 14 doenças e a classe No Finding.

Arquitetura de pastas (Raio-X):

```
imagens/
└── rx/
    ├── nenhuma_doenca_encontrada/
    ├── cardiomegalia/
    ├── fibrose/
    ├── hernia/
    ├── pleural/
    ├── pneumonia/
    └── pneumothorax/
```
#### 3. Angiogramas coronarianos

Tipo de exame: Angiogramas coronarianos  
Número de imagens: 6.126 imagens  
Formato: PNG  
Fonte: Mendeley Data — CADICA: a new dataset for coronary artery disease  
Link da fonte: https://data.mendeley.com/datasets/p9bpx9ctcv/2  
Licença: CC BY 4.0

Para a modalidade de angiografia coronariana, foi escolhida a base CADICA, disponibilizada no Mendeley Data. Essa base é composta por exames de angiografia coronária invasiva de 42 pacientes e fornece os dados em uma estrutura com frames em PNG, arquivos .txt e metadados. Após a auditoria e organização da base, foram consideradas 6.126 imagens válidas no total, sendo 3.996 imagens classificadas como com lesão e 2.130 imagens classificadas como sem lesão, abrangendo 42 pacientes únicos.

Arquitetura de pastas (Angiograma):

```
imagens/
└── angiograma/
    ├── com_lesao/
    │   ├── p1/
    │   ├── p2/
    │   ├── p3/
    │   └── ...
    ├── sem_lesao/
    │   ├── p1/
    │   ├── p2/
    │   ├── p3/
    │   └── ...
    └── labels.csv
```

### Resumo das Fontes Utilizadas

| Tipo de exame | Base escolhida | Quantidade |
|---------------|----------------|------------|
| ECG | ECG Images dataset of Cardiac Patients (Mendeley Data) | 928 imagens |
| Raio-X torácico | NIH Chest X ray-14 (Kaggle, pastas por doença) | 75.168 imagens selecionadas |
| Angiograma coronariano | CADICA (Mendeley Data) | 6.126 imagens |

### Aplicações em Visão Computacional

| Técnica de VC | Descrição | Relevância para o CardioIA |
|---------------|-----------|---------------------------|
| **Detecção de padrões** | Algoritmos de deep learning (CNNs) podem aprender a reconhecer padrões visuais recorrentes em exames cardiológicos, como ritmos específicos em ECGs ou calcificações em raios-X. | Permite triagem automatizada de exames, identificando rapidamente casos que necessitam de atenção especializada e reduzindo o tempo de diagnóstico. |
| **Identificação de bordas e segmentação** | Técnicas como filtros de Canny, U-Net e Mask R-CNN permitem delimitar estruturas anatômicas (câmaras cardíacas, vasos, contornos pulmonares) em imagens médicas. | A segmentação precisa de estruturas cardíacas é essencial para medições automatizadas (ex.: tamanho do coração em raio-X) e para alimentar modelos de diagnóstico. |
| **Reconhecimento de anomalias** | Modelos treinados podem classificar imagens como "normal" ou "anormal", ou identificar anomalias específicas como cardiomegalia, arritmias ou estenoses. | Funciona como uma segunda opinião digital para o cardiologista, reduzindo erros diagnósticos e priorizando exames com achados suspeitos. |
| **Classificação de imagens** | Redes neurais convolucionais podem categorizar exames em diferentes diagnósticos (ex.: fibrilação atrial vs. ritmo sinusal em ECGs). | Viabiliza diagnósticos automatizados em larga escala, especialmente útil em cenários de telemedicina e monitoramento remoto. |
| **Detecção de objetos** | Localização de estruturas ou artefatos específicos dentro da imagem (ex.: localização de stents em angiogramas). | Auxilia no acompanhamento pós-procedimento e na identificação de complicações em exames de follow-up. |

### Justificativa

A Visão Computacional aplicada a exames cardiológicos tem potencial transformador para a saúde. Modelos de IA treinados com imagens médicas podem auxiliar na detecção precoce de doenças, na padronização de laudos, no rastreamento populacional e na redução da sobrecarga de profissionais de saúde. Em regiões com escassez de cardiologistas, um sistema de VC pode oferecer pré-diagnósticos que orientam o encaminhamento de pacientes, democratizando o acesso a cuidados especializados.

---

## Fase 2 — Parte 1: Frases de Sintomas + Extração de Informações

### Descrição

Nesta etapa, avançamos do levantamento de dados para a **construção de um primeiro protótipo de apoio ao diagnóstico cardiovascular baseado em texto**. O objetivo é simular o processo pelo qual um sistema inteligente interpreta o relato de um paciente em linguagem natural e, a partir dele, identifica sintomas e sugere possíveis doenças cardiovasculares.

O protótipo é composto por três artefatos principais, todos versionados neste repositório:

- `assets/sintomas_pacientes.txt` — 10 frases em linguagem natural que simulam relatos de pacientes em uma triagem clínica.
- `assets/mapa_conhecimento.csv` — mapa de conhecimento (ontologia simplificada) com 25 regras que associam conjuntos de sintomas a doenças cardiovasculares.
- `src/diagnostico_cardiovascular.py` — script em Python que lê as frases, normaliza o texto, casa os sintomas contra o mapa de conhecimento e sugere os diagnósticos mais prováveis para cada paciente.

### Estrutura dos Arquivos

#### Frases de sintomas (`assets/sintomas_pacientes.txt`)

Arquivo de texto com uma frase por linha, cada uma representando o relato livre de um paciente. As frases cobrem quadros clínicos diversos, como dor torácica típica, dispneia aos esforços, edema de membros inferiores, palpitações, crise hipertensiva e sinais sugestivos de infarto agudo do miocárdio.

#### Mapa de conhecimento (`assets/mapa_conhecimento.csv`)

Arquivo CSV estruturado com as colunas `Sintoma1`, `Sintoma2`, `Sintoma3` e `DoencaAssociada`. Cada linha define uma regra de associação entre um conjunto de sintomas e uma doença cardiovascular.

Doenças contempladas no mapa:

- Infarto Agudo do Miocárdio
- Insuficiência Cardíaca / Insuficiência Cardíaca Congestiva
- Hipertensão Arterial Sistêmica / Crônica
- Crise Hipertensiva
- Angina Pectoris / Angina Instável
- Arritmia Cardíaca
- Acidente Vascular Cerebral
- Síndrome Coronariana Aguda
- Doença Arterial Coronariana
- Hemorragia Encefálica
- Cardiomiopatia
- Embolia Pulmonar

O mapa foi construído com base em sinais e sintomas descritos nos artigos científicos coletados na Parte 2 (NLP), garantindo rastreabilidade clínica das regras.

#### Script de diagnóstico (`src/diagnostico_cardiovascular.py`)

Script em Python puro (sem dependências externas) que implementa o pipeline:

1. **Carregamento do mapa de conhecimento** a partir do CSV, transformando cada linha em uma regra `{sintomas, doença}`.
2. **Carregamento das frases** dos pacientes a partir do TXT.
3. **Normalização textual** (minúsculas e remoção de acentos) para melhorar a correspondência entre o relato do paciente e os sintomas do mapa.
4. **Identificação de sintomas** via busca por substring dos sintomas normalizados no texto do paciente.
5. **Agrupamento por doença** e cálculo de uma pontuação de confiança baseada na quantidade de sintomas compatíveis.
6. **Sugestão do diagnóstico mais provável** para cada paciente, listando também os diagnósticos alternativos e os sintomas identificados em cada caso.

### Como Executar

```bash
python3 src/diagnostico_cardiovascular.py
```

O script pode ser executado a partir de qualquer diretório — os caminhos dos arquivos `assets/sintomas_pacientes.txt` e `assets/mapa_conhecimento.csv` são resolvidos em relação à localização do próprio script.

### Exemplo de Saída

```
PACIENTE 1:
Relato: "Há dois dias estou com uma dor forte no peito que piora quando faço
esforço físico, sinto um aperto no tórax e falta de ar ao subir escadas."

  Sintomas identificados e diagnósticos sugeridos:
  → Infarto Agudo do Miocardio (confiança: 1 sintoma(s) compatível(is))
    Sintomas encontrados: aperto no torax
  → Insuficiencia Cardiaca Congestiva (confiança: 1 sintoma(s) compatível(is))
    Sintomas encontrados: falta de ar
  → Angina Instavel (confiança: 1 sintoma(s) compatível(is))
    Sintomas encontrados: falta de ar

  ★ DIAGNÓSTICO MAIS PROVÁVEL: Infarto Agudo do Miocardio
```

### Justificativa

Esta Parte 1 da Fase 2 materializa a ponte entre **dados textuais (NLP)** e **apoio à decisão clínica**. Ao transformar relatos não estruturados em diagnósticos sugeridos por meio de um mapa de conhecimento explícito, o protótipo demonstra, em escala reduzida, o mesmo princípio por trás de sistemas mais sofisticados de triagem automatizada e raciocínio clínico. Apesar de educacional e não substituir avaliação médica, o componente estabelece a base sobre a qual técnicas mais avançadas (NER, embeddings, LLMs) serão aplicadas nas próximas entregas do CardioIA.

---

## Referências

<!-- Listar aqui todas as fontes utilizadas para obter os dados e as referências bibliográficas consultadas -->

1. <!-- Preencher -->
2. <!-- Preencher -->
3. <!-- Preencher -->

---

## Licença

Este projeto é de caráter acadêmico, desenvolvido para fins educacionais no curso de Inteligência Artificial da FIAP.
