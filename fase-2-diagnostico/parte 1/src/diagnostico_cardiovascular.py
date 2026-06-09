"""
Sistema Básico de Apoio ao Diagnóstico Cardiovascular
=====================================================
Este script lê frases de sintomas relatados por pacientes (arquivo .txt),
identifica sintomas com base em um mapa de conhecimento (arquivo .csv)
e sugere possíveis diagnósticos cardiovasculares.

Baseado em artigos científicos sobre:
- Hipertensão Arterial Sistêmica
- Infarto Agudo do Miocárdio (MINOCA e obstrutivo)
- Insuficiência Cardíaca
- Hemorragias Encefálicas
- Fatores de risco cardiovascular
- Cardiomiopatia Hipertrófica
"""

import csv
import re
from collections import defaultdict


def carregar_mapa_conhecimento(caminho_csv: str) -> list[dict]:
    """
    Carrega o mapa de conhecimento (ontologia) a partir do arquivo CSV.
    Cada linha contém sintomas associados a uma doença.
    """
    mapa = []
    with open(caminho_csv, "r", encoding="utf-8") as f:
        leitor = csv.DictReader(f)
        for linha in leitor:
            sintomas = []
            for chave, valor in linha.items():
                if chave.startswith("Sintoma") and valor:
                    sintomas.append(valor.strip().lower())
            doenca = linha.get("DoencaAssociada", "").strip()
            if sintomas and doenca:
                mapa.append({"sintomas": sintomas, "doenca": doenca})
    return mapa


def carregar_frases(caminho_txt: str) -> list[str]:
    """
    Carrega as frases de sintomas dos pacientes a partir do arquivo .txt.
    Cada linha representa a descrição de um paciente.
    """
    with open(caminho_txt, "r", encoding="utf-8") as f:
        frases = [linha.strip() for linha in f if linha.strip()]
    return frases


def normalizar_texto(texto: str) -> str:
    """
    Normaliza o texto removendo acentos comuns e convertendo para minúsculas.
    Isso melhora a correspondência entre sintomas do mapa e as frases.
    """
    texto = texto.lower()
    substituicoes = {
        "á": "a", "à": "a", "ã": "a", "â": "a",
        "é": "e", "ê": "e",
        "í": "i",
        "ó": "o", "ô": "o", "õ": "o",
        "ú": "u", "ü": "u",
        "ç": "c",
    }
    for original, substituto in substituicoes.items():
        texto = texto.replace(original, substituto)
    return texto


def identificar_sintomas_e_diagnosticar(
    frase: str, mapa: list[dict]
) -> list[dict]:
    """
    Para cada frase de paciente, verifica quais sintomas do mapa de
    conhecimento estão presentes e retorna os diagnósticos sugeridos
    com a pontuação (quantidade de sintomas encontrados).
    """
    frase_normalizada = normalizar_texto(frase)
    resultados = []

    for entrada in mapa:
        sintomas_encontrados = []
        for sintoma in entrada["sintomas"]:
            sintoma_normalizado = normalizar_texto(sintoma)
            if sintoma_normalizado in frase_normalizada:
                sintomas_encontrados.append(sintoma)

        if sintomas_encontrados:
            resultados.append({
                "doenca": entrada["doenca"],
                "sintomas_identificados": sintomas_encontrados,
                "pontuacao": len(sintomas_encontrados),
            })

    # Ordena por pontuação decrescente (mais sintomas = mais relevante)
    resultados.sort(key=lambda x: x["pontuacao"], reverse=True)
    return resultados


def executar_diagnostico(caminho_txt: str, caminho_csv: str) -> None:
    """
    Função principal que orquestra o processo de diagnóstico:
    1. Carrega o mapa de conhecimento
    2. Carrega as frases dos pacientes
    3. Para cada frase, identifica sintomas e sugere diagnósticos
    """
    print("=" * 70)
    print("SISTEMA DE APOIO AO DIAGNÓSTICO CARDIOVASCULAR")
    print("=" * 70)

    # Carregar dados
    mapa = carregar_mapa_conhecimento(caminho_csv)
    print(f"\nMapa de conhecimento carregado: {len(mapa)} regras de associação")

    frases = carregar_frases(caminho_txt)
    print(f"Frases de pacientes carregadas: {len(frases)} descrições\n")

    # Processar cada frase
    for i, frase in enumerate(frases, 1):
        print("-" * 70)
        print(f"PACIENTE {i}:")
        print(f"Relato: \"{frase}\"")
        print()

        resultados = identificar_sintomas_e_diagnosticar(frase, mapa)

        if resultados:
            # Agrupar diagnósticos por doença (pode haver múltiplas regras
            # que apontam para a mesma doença)
            diagnosticos_agrupados = defaultdict(list)
            for r in resultados:
                diagnosticos_agrupados[r["doenca"]].extend(
                    r["sintomas_identificados"]
                )

            # Remover duplicatas de sintomas e calcular pontuação final
            diagnosticos_finais = []
            for doenca, sintomas in diagnosticos_agrupados.items():
                sintomas_unicos = list(set(sintomas))
                diagnosticos_finais.append({
                    "doenca": doenca,
                    "sintomas": sintomas_unicos,
                    "pontuacao": len(sintomas_unicos),
                })

            diagnosticos_finais.sort(
                key=lambda x: x["pontuacao"], reverse=True
            )

            print("  Sintomas identificados e diagnósticos sugeridos:")
            for d in diagnosticos_finais:
                print(f"  → {d['doenca']} (confiança: {d['pontuacao']} "
                      f"sintoma(s) compatível(is))")
                print(f"    Sintomas encontrados: {', '.join(d['sintomas'])}")

            melhor = diagnosticos_finais[0]
            print(f"\n  ★ DIAGNÓSTICO MAIS PROVÁVEL: {melhor['doenca']}")
        else:
            print("  ⚠ Nenhum sintoma reconhecido no mapa de conhecimento.")
            print("  Recomenda-se avaliação clínica detalhada.")

        print()

    print("=" * 70)
    print("NOTA: Este sistema é apenas um apoio educacional e não substitui")
    print("a avaliação de um profissional de saúde qualificado.")
    print("=" * 70)


# === EXECUÇÃO PRINCIPAL ===
if __name__ == "__main__":
    CAMINHO_TXT = "sintomas_pacientes.txt"
    CAMINHO_CSV = "mapa_conhecimento.csv"

    executar_diagnostico(CAMINHO_TXT, CAMINHO_CSV)
