struct Data {
  char unit[10];
  char days[DAYS][20];
  unsigned int values[DAYS];
};

void fillDataFromJson(Data* data) {
  sprintf(data->unit, "%s", (const char*) json["unit"]);

  for (int i = 0; i < DAYS; i++) {
    sprintf(data->days[i], "%s", (const char*) json["data"][i]["date"]);
    data->values[i] = (unsigned int) int(json["data"][i]["value"]);
  }
}

/* Daily.json
{
    "unit": "Wh",
    "data": [
        {
            "date": "2021-01-17",
            "value": 3500
        },
        ...
    ]
}
*/

/* maxpower.json
{
    "unit": "VA",
    "data": [
        {
            "date": "2021-01-17 09:22:00",
            "value": 3500
        },
 
    ]
}
*/
