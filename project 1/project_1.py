import math
import numbers
import time

class Colors:
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    BOLD = '\033[1m'
    END = '\033[0m'


def calculeaza_distanta_euclidiana(p1, p2):
    """Distanța în metri (presupunem 1 unitate = 1 metru)"""
    dx = p2['x'] - p1['x']
    dy = p2['y'] - p1['y']
    return math.sqrt(dx ** 2 + dy ** 2)


def calculeaza_timp_mers_pe_jos(distanta_metri):
    """ Calculează timp mers pe jos Viteza medie: 5 km/h = 83.33 m/min"""
    viteza_m_pe_min = 83.33
    timp_minute = distanta_metri / viteza_m_pe_min
    return timp_minute


def analizeaza_statie(statie, pozitie_student):
    """ Analizează o stație și returnează timpul total minim """
    distanta_la_statie = calculeaza_distanta_euclidiana(pozitie_student, statie)
    timp_mers = calculeaza_timp_mers_pe_jos(distanta_la_statie)

    # Analizăm fiecare linie de autobuz
    optiuni_linii = []
    for linie in statie['linii']:
        timp_asteptare_mediu = linie['frecventa_min'] / 2
        timp_calatorie = linie['durata_calatorie_min']
        timp_total = timp_mers + timp_asteptare_mediu + timp_calatorie

        optiuni_linii.append({
            'linie': linie['numar'],
            'timp_mers': timp_mers,
            'timp_asteptare': timp_asteptare_mediu,
            'timp_calatorie': timp_calatorie,
            'timp_total': timp_total
        })

    # Găsim cea mai rapidă linie de la această stație
    cea_mai_rapida = min(optiuni_linii, key=lambda o: o['timp_total'])

    return {
        'statie': statie['nume'],
        'distanta_la_statie_m': distanta_la_statie,
        'cea_mai_rapida_optiune': cea_mai_rapida,
        'toate_optiunile': optiuni_linii
    }


def is_number(value):
    return isinstance(value, numbers.Number) and not isinstance(value, bool)


def line_is_valid(line):
    return line['frecventa_min'] > 0 and line['durata_calatorie_min'] > 0


def station_is_valid(station):
    if not is_number(station['x']) or not is_number(station['y']):
        print(f"{station['nume']} has wrong coordinates")
        return False

    # Skip invalid lines
    station['linii'] = [line for line in station['linii'] if line_is_valid(line)]

    # Verify if this station has lines
    if len(station['linii']) == 0:
        print(f"{station['nume']} doesn't have any lines")
        return False
    return True


def gaseste_statie_optima(statii, pozitie_student):
    analize = []
    for statie in statii:
        # Skip invalid stations
        if not station_is_valid(statie):
            continue
        analiza = analizeaza_statie(statie, pozitie_student)
        analize.append(analiza)
        # Sortăm după timp total
    return sorted(analize, key=lambda a: a['cea_mai_rapida_optiune']['timp_total'])


def afiseaza_interfata_vizuala(rezultate):

    print(f"\n{Colors.BOLD}╔════════════════════════════════════════════════════════════╗")
    print(f"║               REZULTATE CĂUTARE STAȚIE OPTIMĂ              ║")
    print(f"╚════════════════════════════════════════════════════════════╝{Colors.END}\n")

    for i, analiza in enumerate(rezultate, 1):
        optiune = analiza['cea_mai_rapida_optiune']
        t_mers = optiune['timp_mers']
        t_astept = optiune['timp_asteptare']
        t_drum = optiune['timp_calatorie']

        color_theme = Colors.GREEN if i == 1 else ""

        print(f"{color_theme}{Colors.BOLD}{i}. {analiza['statie']} (Linie recomandată: {optiune['linie']}){Colors.END}")
        print(f" Timp total: {optiune['timp_total']:.1f} min")
        print(f" {Colors.BLUE}🚶Mers pe jos: {t_mers:.1f} min{Colors.END}")
        print(f" {Colors.YELLOW}⏳Timp mediu de așteptare: {t_astept:.1f} min{Colors.END}")
        print(f" {Colors.RED}🚌Durata călătoriei: {t_drum} min{Colors.END}\n")


    # Program principal
def main_autobuz():
    pozitie_student = {"x": 100, "y": 150}
    statii = [
        {
            "nume": "Stația Piață",
            "x": 120, "y": 180,
            "linii": [
                {"numar": "21", "frecventa_min": 10, "durata_calatorie_min": 25},
                {"numar": "7", "frecventa_min": 15, "durata_calatorie_min": 22}
            ]
        },
        {
            "nume": "Stația Gara",
            "x": 90, "y": 200,
            "linii": [{"numar": "15", "frecventa_min": 8, "durata_calatorie_min": 30}]
        },
        {
            "nume": "Stația Universitate",
            "x": 150, "y": 140,
            "linii": [
                {"numar": "5", "frecventa_min": 12, "durata_calatorie_min": 20},
                {"numar": "11", "frecventa_min": 20, "durata_calatorie_min": 18}
            ]
        },
    ]

    print("=== 🔎CĂUTARE STAȚIE OPTIMĂ ===\n")
    time.sleep(1)
    print(f"  {Colors.GREEN}{Colors.END} Identificate {len(statii)} stații.")
    print(f"Poziția ta: ({pozitie_student['x']}, {pozitie_student['y']})\n")
    rezultate = gaseste_statie_optima(statii, pozitie_student)

    if rezultate:
        afiseaza_interfata_vizuala(rezultate)
    else:
        print("Nu s-au găsit stații valide.")


if __name__ == "__main__":
    main_autobuz()