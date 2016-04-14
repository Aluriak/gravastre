from __future__ import print_function, division
import json
from itertools import islice
from collections import namedtuple, defaultdict
from SPARQLWrapper import SPARQLWrapper, JSON


PLANET_FIELDS = ('name', 'mass', 'eccentricity', 'semimajoraxis')
PLANET_DATA = defaultdict(dict, {  # data not retrievable from wikidata
    'sun': {'color': [255, 255, 0]},
    'mercury': {'color': [128, 128, 128]},
    'venus': {'color': [170, 165, 141]},
    'earth': {'color': [0, 0, 255]},
    'moon': {'color': [255, 255, 255]},
    'mars': {'color': [255, 0, 0]},
    # 'Jupiter': {'color': []},
    # 'Saturn': {'color': []},
    # 'Uranus': {'color': []},
    # 'Neptune': {'color': []},
})


PREFIXES = """
PREFIX wikibase: <http://wikiba.se/ontology#>
PREFIX wdt: <http://www.wikidata.org/prop/direct/>
PREFIX wd: <http://www.wikidata.org/entity/>

"""
QUERY = """

SELECT ?object ?objectLabel ?mass ?eccentricity ?semimajoraxis
WHERE {

  ?object wdt:P397 wd:Q525.  # the object should have the sun as parent astronomical body

  # Object properties
  ?object wdt:P31 ?status.
  ?object wdt:P2067 ?mass.
  ?object wdt:P1096 ?eccentricity.
  ?object wdt:P2233 ?semimajoraxis.

  # Here are the accepted object status
  VALUES ?status {wd:Q128207 wd:Q121750 wd:Q1319599}.  # terrestrial planet, gas giant, ice giant

  # automatically use the english label for ?xLabel variables
  SERVICE wikibase:label { bd:serviceParam wikibase:language "en" }
}

"""


def query(query, prefixes=PREFIXES):
    """Ask Wikidata with given query"""
    sparql = SPARQLWrapper('https://query.wikidata.org/sparql')
    sparql.setQuery(prefixes + QUERY)
    sparql.setReturnFormat(JSON)
    results = sparql.query().convert()
    for result in results['results']['bindings']:
        yield result


def yotta_to_kilo(yotta):
    """Yotta is 1e24, kilo is 1e3"""
    return float(yotta) * (1e24 / 1e3)

def semimajoraxis_in_au(value):
    """Try to guess if given value is in kilometer or AU, returns AU"""
    # if low enough, it's probably AU, else kilometer
    return float(value) / (1 if float(value) < 1000 else 150e6)

def normalized_planet(planet):
    """Return a new planet, normalized"""
    return Planet(
        name=str(planet.name).lower(),
        mass=float(yotta_to_kilo(planet.mass)),
        semimajoraxis=float(semimajoraxis_in_au(planet.semimajoraxis)),
        eccentricity=float(planet.eccentricity),
    )


def json_planet(planet):
    return json.dumps({
        **PLANET_DATA[planet.name.lower()],
        **{f.lower(): getattr(planet, f)
           for f in PLANET_FIELDS},
    })


if __name__ == "__main__":
    Planet = namedtuple('Planet', PLANET_FIELDS)
    planets = set()
    for result in query(QUERY):
        result = {f: v['value'] for f, v in result.items()}
        new = normalized_planet(Planet(
            result['objectLabel'], result['mass'],
            result['eccentricity'],
            semimajoraxis_in_au(result['semimajoraxis'])
        ))
        if not any(new.name == p.name for p in planets):
            planets.add(new)
    for planet in planets:
        print(json_planet(planet))
