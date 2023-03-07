# BOTanical

|List of States:			|Options:	|Next state:|
|---------------------|-------------|-----------|
|				              |(default 1)	|pointer starts at 0|
|-1 = ERROR			|		|0|
|0.	Main menu		|6		|pointer+1 (goes to 1~6)|
|1.	Overview		|		|0|
|2.	Sensor Readings		|5		|if pointer == 4 -> 0 ; else pointer+7 (goes to 7~10)|
|3.	Notifications		|		|11|
|4.	Already Watered		|2		|pointer+12
|5.	Select Plant		|10		|14|
|6.	Credits			|		|-1|
|7.	Light sensor		|		|2|
|8.	Moisture sensor		|		|2|
|9.	Temperature sensor	|		|2|
|10.	Humidity sensor		|		|2|
|11.	Notifications read?	|2		|0|
|12.	Thanks for watering|			|0|
|13.	Botanical will water	|		|0|
|14.	Plant Details		|2		|0|
