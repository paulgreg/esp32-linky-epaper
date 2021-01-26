struct Data {
  char unit[10];
  char days[DAYS][20];
  unsigned int values[DAYS];
};

boolean fillDataFromJson(Data* data) {
  sprintf(data->unit, "%s", (const char*) json["unit"]);
  int size = json["data"].length();
  Serial.printf("data size: %i\0", size);
  if (size < DAYS) return false;
  for (int i = 0, id = size - DAYS; i < DAYS; i++, id++) {
    sprintf(data->days[i], "%s", (const char*) json["data"][id]["date"]);
    data->values[i] = (unsigned int) int(json["data"][id]["value"]);
  }
  return true;
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
