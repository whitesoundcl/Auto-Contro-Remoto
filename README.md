# Auto-Contro-Remoto
Un repositorio de un auto a control remoto operado por un NodeMCU

Si bien quizá no estas interesado en construir uno, quizá puedas aprender algo nuevo
revisando el código.
![alt text](https://raw.githubusercontent.com/whitesoundcl/Auto-Contro-Remoto/master/imagenes/imagen1.jpg "El auto en cuestión XD")

https://youtu.be/uagsGugIrPU Video demostrativo

Hay un poco más de información escrita en mi blog
https://blog.luch-in.cl/2019/02/proyecto-auto-control-remoto-con-nodemcu.html
___
## ¿Cómo funciona?
El vehiculo se mueve a base de peticiones http con paso de parámetros.
por ejemplo si ingresamos a la siguiente url:
```
<Direccion ip del nodeMCU>/controlar?motor=160&dir=70
```
el motor del vehiculo se moverá a una velocidad de 160 y el servo que controla la
dirección será puesto en un ángulo de 70.

Como controlar el vehiculo en base a parámetros en una url puede llegar a ser incómodo,
también creé una página que permite controlar el vehiculo con las teclas direccionales.
El código javascript mandará peticiones con los parametros correctos utilizando AJAX.



Los materiales que ocupé para la creación de este proyecto fueron:
+ NodeMCU LoLin
+ Pantalla OLED (opcional)
+ 1 servomotor (Direccion)
+ 1 controlador brushed Walkera (Motor)
+ 1 chasís de auto a control remoto.
+ cargador portatíl para teléfono.
+ 2 elevadores de voltaje
+ Un montón de cable

Cualquier duda o consulta, estoy siempre disponible.
