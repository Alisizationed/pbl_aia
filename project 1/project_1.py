import math  

def calculeaza_distanta_euclidiana(p1, p2): 
  """Distanța în metri (presupunem 1 unitate = 1 metru)"""
  dx = p2['x'] - p1['x']
  dy = p2['y'] - p1['y'] 

  return math.sqrt(dx**2 + dy**2)  
  
def calculeaza_timp_mers_pe_jos(distanta_metri):
  """ Calculează timp mers pe jos Viteza medie: 5 km/h = 83.33 m/min"""
  viteza_m_pe_min = 83.33
  timp_minute = distanta_metri / viteza_m_pe_min 

  return timp_minute

def analizeaza_statie(statie, pozitie_student):
  """ Analizează o stație și returnează timpul total minim """
  # Distanța până la stație
  distanta_la_statie = calculeaza_distanta_euclidiana(pozitie_student,  statie)
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

def gaseste_statie_optima(statii, pozitie_student): 
  """ Analizează toate stațiile și găsește opțiunea optimă """
  analize = [] 

  for statie in statii:
    analiza = analizeaza_statie(statie, pozitie_student)
    analize.append(analiza)

  # Sortăm după timp total
  analize_sortate = sorted(analize, key=lambda a: a['cea_mai_rapida_optiune']['timp_total'])

  return analize_sortate  

# Program principal 
def main_autobuz(): 
  
  pozitie_student = {"x": 100, "y": 150}
  statii = [{
    "nume": "Stația Piață", 
    "x": 120, 
    "y": 180, 
    "linii": 
      [{
          "numar": "21", 
          "frecventa_min": 10,
          "durata_calatorie_min": 25
      }, 
      {
          "numar": "7",
          "frecventa_min": 15,
          "durata_calatorie_min": 22
          }
      ]
    },
    {
      "nume": "Stația Gara", 
      "x": 90,
      "y": 200, 
      "linii": 
      [{
          "numar": "15",
          "frecventa_min": 8,
          "durata_calatorie_min": 30
          }
        ]
      }, 
    {
      "nume": "Stația Universitate", 
      "x": 150, 
      "y": 140, 
      "linii": 
      [{
          "numar": "5",
          "frecventa_min": 12, 
          "durata_calatorie_min": 20
          }, 
        {
          "numar": "11",
          "frecventa_min": 20,
          "durata_calatorie_min": 18
        }] 
      }, 
    ]

  print("=== CĂUTARE STAȚIE OPTIMĂ ===\n")
  print(f"Poziția ta: ({pozitie_student['x']}, {pozitie_student['y']})\n")
  rezultate = gaseste_statie_optima(statii, pozitie_student) 
  print(f"Analizate {len(statii)} stații:\n") 

  for i, analiza in enumerate(rezultate, 1):

    optiune = analiza['cea_mai_rapida_optiune']
    print(f"{i}. {analiza['statie']}")
    print(f"   Distanță: {analiza['distanta_la_statie_m']:.0f}m")
    print(f"   Linie recomandată: {optiune['linie']}")
    print(f"   └─ Mers pe jos: {optiune['timp_mers']:.1f} min")
    print(f"   └─ Așteptare medie: {optiune['timp_asteptare']:.1f} min")
    print(f"   └─ Călătorie: {optiune['timp_calatorie']} min") 
    print(f"   TOTAL: {optiune['timp_total']:.1f} minute\n") 

  # Recomandare finală     
  cea_mai_buna = rezultate[0]
  print("="*50) 
  print("✓ RECOMANDARE:")
  print(f"Mergi la {cea_mai_buna['statie']}")
  print(f"Ia autobuzul {cea_mai_buna['cea_mai_rapida_optiune']['linie']}")
  print(f"Timp total estimat: {cea_mai_buna['cea_mai_rapida_optiune']['timp_total']:.1f} minute")  

if __name__ == "__main__":
  main_autobuz()