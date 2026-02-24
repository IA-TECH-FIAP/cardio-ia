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
├── README.md              # Este arquivo
├── docs/                  # Textos médicos para NLP (Parte 2)
│   ├── texto1.txt         # (a definir)
│   └── texto2.txt         # (a definir)
└── ...
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

Nesta etapa, buscamos e organizamos um dataset com **no mínimo 100 registros** contendo variáveis clínicas associadas a doenças cardiovasculares. Esses dados serão a base para treinamento de modelos de Machine Learning nas fases posteriores.

### Link para os Dados

> **Dataset completo:** [Inserir link público do Google Drive / OneDrive aqui]
<!-- Exemplo: https://drive.google.com/drive/folders/XXXXX -->

### Origem dos Dados

<!-- Preencher com a origem real dos dados. Exemplos de fontes possíveis:
- UCI Heart Disease Dataset (https://archive.ics.uci.edu/dataset/45/heart+disease)
- Kaggle Heart Failure Prediction Dataset
- Dados simulados gerados pela equipe (descrever o método de geração)
-->

**Fonte:** <!-- Preencher -->
**Tipo:** <!-- Real / Simulado / Misto -->
**Formato:** <!-- CSV / XLSX / JSON -->
**Número de registros:** <!-- Preencher (mínimo 100) -->

### Variáveis do Dataset

A tabela abaixo descreve as variáveis esperadas e sua relevância clínica:

| Variável | Descrição | Relevância Clínica |
|----------|-----------|---------------------|
| `age` | Idade do paciente (anos) | A idade é um dos principais fatores de risco cardiovascular. A incidência de doenças cardíacas aumenta significativamente após os 45 anos em homens e 55 anos em mulheres. |
| `sex` | Sexo biológico (M/F) | Homens e mulheres apresentam perfis de risco e sintomatologias distintas para doenças cardiovasculares, o que impacta diretamente o diagnóstico e o tratamento. |
| `cp` | Tipo de dor torácica | A classificação da dor torácica (típica, atípica, não anginosa, assintomática) é um indicador-chave na triagem de síndromes coronarianas agudas. |
| `trestbps` | Pressão arterial em repouso (mmHg) | Hipertensão é um dos principais fatores de risco modificáveis para insuficiência cardíaca, AVC e doença arterial coronariana. |
| `chol` | Colesterol sérico (mg/dL) | Níveis elevados de colesterol LDL estão diretamente associados à formação de placas ateroscleróticas, aumentando o risco de infarto. |
| `fbs` | Glicemia em jejum > 120 mg/dL | Diabetes mellitus é um fator de risco independente para doenças cardiovasculares, acelerando processos ateroscleróticos. |
| `restecg` | Resultado do ECG em repouso | Alterações eletrocardiográficas em repouso podem indicar hipertrofia ventricular, isquemia ou arritmias subjacentes. |
| `thalach` | Frequência cardíaca máxima atingida | A incapacidade de atingir uma frequência cardíaca adequada durante esforço pode indicar insuficiência cronotrópica, associada a maior mortalidade cardiovascular. |
| `exang` | Angina induzida por exercício | A presença de angina durante esforço físico é um marcador clássico de isquemia miocárdica, sugerindo obstrução coronariana significativa. |
| `oldpeak` | Depressão do segmento ST | Depressão do segmento ST durante exercício é um indicador eletrocardiográfico de isquemia, com valor prognóstico para eventos cardíacos adversos. |
| `target` | Presença de doença cardíaca (0/1) | Variável-alvo para modelos de classificação: permite treinar algoritmos de ML para prever a probabilidade de doença cardiovascular. |

> **Nota:** As variáveis listadas acima são representativas. A equipe deve ajustar esta tabela de acordo com o dataset efetivamente selecionado.

### Justificativa para IA

Essas variáveis permitem construir modelos preditivos de risco cardiovascular usando algoritmos como Regressão Logística, Random Forest, XGBoost e redes neurais. As variáveis contínuas (pressão, colesterol, frequência cardíaca) podem servir como entrada para sistemas de monitoramento IoT em tempo real, enquanto as categóricas (tipo de dor, resultado de ECG) auxiliam na triagem automatizada de pacientes.

---

## Parte 2 — Dados Textuais (NLP)

### Descrição

Nesta etapa, reunimos **no mínimo dois textos** (`.txt`) relacionados a doenças cardíacas, saúde pública cardiovascular, sintomas ou tratamentos. Os textos estão armazenados na subpasta `docs/` deste repositório.

### Textos Coletados

| Arquivo | Título / Descrição | Fonte |
|---------|-------------------|-------|
| `docs/texto1.txt` | <!-- Preencher --> | <!-- Preencher (ex.: SciELO, BVS, SUS, Projeto Gutenberg) --> |
| `docs/texto2.txt` | <!-- Preencher --> | <!-- Preencher --> |

<!-- Adicionar mais linhas conforme necessário -->

### Fontes Sugeridas

- **SciELO** (Scientific Electronic Library Online): https://scielo.org
- **BVS** (Biblioteca Virtual em Saúde): https://bvsalud.org
- **Ministério da Saúde / SUS**: https://www.gov.br/saude
- **Projeto Gutenberg**: https://www.gutenberg.org
- **PubMed Central**: https://www.ncbi.nlm.nih.gov/pmc/

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

Nesta etapa, reunimos **no mínimo 100 imagens** (`.jpg` ou `.png`) de exames cardiológicos, como eletrocardiogramas (ECGs), angiogramas coronarianos ou raios-X torácicos.

### Link para as Imagens

> **Imagens completas:** [Inserir link público do Google Drive / OneDrive aqui]
<!-- Exemplo: https://drive.google.com/drive/folders/XXXXX -->

### Tipo de Exame Selecionado

<!-- Preencher com o tipo de exame escolhido pela equipe. Exemplos:
- Eletrocardiogramas (ECG)
- Raio-X torácico
- Angiogramas coronarianos
- Ecocardiogramas
-->

**Tipo de exame:** <!-- Preencher -->
**Número de imagens:** <!-- Preencher (mínimo 100) -->
**Formato:** <!-- JPG / PNG -->
**Fonte:** <!-- Preencher (ex.: PhysioNet, Kaggle, NIH Chest X-rays, etc.) -->

### Fontes Sugeridas

- **PhysioNet** (PTB-XL ECG Dataset): https://physionet.org/content/ptb-xl/
- **NIH Chest X-ray Dataset**: https://nihcc.app.box.com/v/ChestXray-NIHCC
- **Kaggle** (diversos datasets de imagens médicas): https://www.kaggle.com
- **MIMIC-CXR** (Chest X-rays): https://physionet.org/content/mimic-cxr/

### Aplicações em Visão Computacional

| Técnica de VC | Descrição | Relevância para o CardioIA |
|---------------|-----------|---------------------------|
| **Detecção de padrões** | Algoritmos de deep learning (CNNs) podem aprender a reconhecer padrões visuais recorrentes em exames cardiológicos, como ritmos específicos em ECGs ou calcificações em raios-X. | Permite triagem automatizada de exames, identificando rapidamente casos que necessitam de atenção especializada e reduzindo o tempo de diagnóstico. |
| **Identificação de bordas e segmentação** | Técnicas como filtros de Canny, U-Net e Mask R-CNN permitem delimitar estruturas anatômicas (câmaras cardíacas, vasos, contornos pulmonares) em imagens médicas. | A segmentação precisa de estruturas cardíacas é essencial para medições automatizadas (ex.: tamanho do coração em raio-X) e para alimentar modelos de diagnóstico. |
| **Reconhecimento de anomalias** | Modelos treinados podem classificar imagens como "normal" ou "anormal", ou identificar anomalias específicas como cardiomegalia, arritmias ou estenoses. | Funciona como uma segunda opinião digital para o cardiologista, reduzindo erros diagnósticos e priorizando exames com achados suspeitos. |
| **Classificação de imagens** | Redes neurais convolucionais podem categorizar exames em diferentes diagnósticos (ex.: fibrilação atrial vs. ritmo sinusal em ECGs). | Viabiliza diagnósticos automatizados em larga escala, especialmente útil em cenários de telemedicina e monitoramento remoto. |
| **Detecção de objetos** | Localização de estruturas ou artefatos específicos dentro da imagem (ex.: localização de stents em angiogramas). | Auxilia no acompanhamento pós-procedimento e na identificação de complicações em exames de follow-up. |

### Justificativa

A Visão Computacional aplicada a exames cardiológicos tem potencial transformador para a saúde. Modelos de IA treinados com imagens médicas podem auxiliar na **detecção precoce de doenças**, na **padronização de laudos**, no **rastreamento populacional** e na **redução da sobrecarga de profissionais de saúde**. Em regiões com escassez de cardiologistas, um sistema de VC pode oferecer pré-diagnósticos que orientam o encaminhamento de pacientes, democratizando o acesso a cuidados especializados.

---

## Referências

<!-- Listar aqui todas as fontes utilizadas para obter os dados e as referências bibliográficas consultadas -->

1. <!-- Preencher -->
2. <!-- Preencher -->
3. <!-- Preencher -->

---

## Licença

Este projeto é de caráter acadêmico, desenvolvido para fins educacionais no curso de Inteligência Artificial da FIAP.
