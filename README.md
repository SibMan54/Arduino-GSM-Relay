# Arduino-GSM-Relay  
Проект GSM реле на Arduino с использованием модуля NEOWAY M590
Проект был написан давно, но небыл никуда опубликован, лежал на ПК
# Функционал реле следующий:  
1. Реле вкл и выкл нагрузку позвонку, проискодит сброс вызова и происходит смена состояния ВКЛ/ВЫКЛ;  
2. Включение и выключение реле через SMS, отправляем смс на устройство с текстом "relay on" "relay off" в ответ получаем смс о статусе;  
3. Включение с таймером через SMS, отправляем смс на устройство с текстом "timer МИН" например "timer 60" включит реле и через 60 мин сам выкл его;  
4. Проверка температуры с датчика DS18b20, отправляем смс на устройство с текстом "temper" в ответ получаем смс с текущей температурой;  
5. Также я попытался реализовать возможность самоподогрева устройства отправкой SMS с текстом "termostat ТЕМПЕРАТУРА ВКЛ ПОДОГРЕВА"
например "termostat -10" если датчик стоит внутри устройства, то при достижении -10 град. вкл самоподогрев;  
6. И самое главное в устройстве реализована защита от сторонних звонков и СМС, в устройстве сохраняются 2 номера тел MASTER и MASTER2 с которых можно управлять устройством.
Имеется смена этих номеров при помощи СМС, для этого отправляем смс с текстом "new master" или "new master2" с номера который вы хотите сделать мастер номером,
то есть если если вы хотите сделать НОМЕР1 сделать первым мастером, то отправляем с него текст "new master" и этот номер станет первым мастер номером и по аналогии со вторым также;
