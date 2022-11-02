PAV - P2: detección de actividad vocal (VAD)
============================================

Esta práctica se distribuye a través del repositorio GitHub [Práctica 2](https://github.com/albino-pav/P2),
y una parte de su gestión se realizará mediante esta web de trabajo colaborativo.  Al contrario que Git,
GitHub se gestiona completamente desde un entorno gráfico bastante intuitivo. Además, está razonablemente
documentado, tanto internamente, mediante sus [Guías de GitHub](https://guides.github.com/), como
externamente, mediante infinidad de tutoriales, guías y vídeos disponibles gratuitamente en internet.


Inicialización del repositorio de la práctica.
----------------------------------------------

Para cargar los ficheros en su ordenador personal debe seguir los pasos siguientes:

*	Abra una cuenta GitHub para gestionar esta y el resto de prácticas del curso.
*	Cree un repositorio GitHub con el contenido inicial de la práctica (sólo debe hacerlo uno de los
	integrantes del grupo de laboratorio, cuya página GitHub actuará de repositorio central del grupo):
	-	Acceda la página de la [Práctica 2](https://github.com/albino-pav/P2).
	-	En la parte superior derecha encontrará el botón **`Fork`**. Apriételo y, después de unos segundos,
		se creará en su cuenta GitHub un proyecto con el mismo nombre (**P2**). Si ya tuviera uno con ese 
		nombre, se utilizará el nombre **P2-1**, y así sucesivamente.
*	Habilite al resto de miembros del grupo como *colaboradores* del proyecto; de este modo, podrán
	subir sus modificaciones al repositorio central:
	-	En la página principal del repositorio, en la pestaña **:gear:`Settings`**, escoja la opción 
		**Collaborators** y añada a su compañero de prácticas.
	-	Éste recibirá un email solicitándole confirmación. Una vez confirmado, tanto él como el
		propietario podrán gestionar el repositorio, por ejemplo: crear ramas en él o subir las
		modificaciones de su directorio local de trabajo al repositorio GitHub.
*	En la página principal del repositorio, localice el botón **Branch: master** y úselo para crear
	una rama nueva con los primeros apellidos de los integrantes del equipo de prácticas separados por
	guion (**fulano-mengano**).
*	Todos los miembros del grupo deben realizar su copia local en su ordenador personal.
	-	Copie la dirección de su copia del repositorio apretando en el botón **Clone or download**.
		Asegúrese de usar *Clone with HTTPS*.
	-	Abra una sesión de Bash en su ordenador personal y vaya al directorio **PAV**. Desde ahí, ejecute:

		```.sh
		git clone dirección-del-fork-de-la-práctica
		```

	-	Vaya al directorio de la práctica `cd P2`.
	-	Añada la etiqueta `origin` a su copia del repositorio. Esto es útil para facilitar los *push* y
		*pull* al repositorio original:
		```.sh
		git remote add origin dirección-del-fork-de-la-práctica
		```
	-	Cambie a la rama **fulano-mengano** con la orden:

		```.sh
		git checkout fulano-mengano
		```

*	A partir de este momento, todos los miembros del grupo de prácticas pueden trabajar en su directorio
	local del modo habitual.
	-	También puede utilizar el repositorio remoto como repositorio central para el trabajo colaborativo
		de los distintos miembros del grupo de prácticas; o puede serle útil usarlo como copia de
		seguridad.
	-	Cada vez que quiera subir sus cambios locales al repositorio GitHub deberá confirmar los
		cambios en su directorio local:

		```.sh
		git add .
		git commit -m "Mensaje del commit"
		```

		y, a continuación, subirlos con la orden:

		```.sh
		git push -u origin fulano-mengano
		```

*	Al final de la práctica, la rama **fulano-mengano** del repositorio GitHub servirá para remitir la
	práctica para su evaluación utilizando el mecanismo *pull request*.
	-	Vaya a la página principal de la copia del repositorio y asegúrese de estar en la rama
		**fulano-mengano**.
	-	Pulse en el botón **New pull request**, y siga las instrucciones de GitHub.


Entrega de la práctica.
-----------------------

Responda, en este mismo documento (README.md), los ejercicios indicados a continuación. Este documento es
un fichero de texto escrito con un formato denominado _**markdown**_. La principal característica de este
formato es que, manteniendo la legibilidad cuando se visualiza con herramientas en modo texto (`more`,
`less`, editores varios, ...), permite amplias posibilidades de visualización con formato en una amplia
gama de aplicaciones; muy notablemente, **GitHub**, **Doxygen** y **Facebook** (ciertamente, :eyes:).

En GitHub. cuando existe un fichero denominado README.md en el directorio raíz de un repositorio, se
interpreta y muestra al entrar en el repositorio.

Debe redactar las respuestas a los ejercicios usando Markdown. Puede encontrar información acerca de su
sintáxis en la página web [Sintaxis de Markdown](https://daringfireball.net/projects/markdown/syntax).
También puede consultar el documento adjunto [MARKDOWN.md](MARKDOWN.md), en el que se enumeran los
elementos más relevantes para completar la redacción de esta práctica.

Recuerde realizar el *pull request* una vez completada la práctica.

Ejercicios
----------

### Etiquetado manual de los segmentos de voz y silencio

- Etiquete manualmente los segmentos de voz y silencio del fichero grabado al efecto. Inserte, a 
  continuación, una captura de `wavesurfer` en la que se vea con claridad la señal temporal, el contorno de
  potencia y la tasa de cruces por cero, junto con el etiquetado manual de los segmentos.

	Tenemos la siguiente gráfica: 
	![Alt text](./wave.png?raw=true "Optional Title")
	

		 Nuestro etiquetado manual es :
	| start | end | value |
	| --- | --- | --- |
	| 0.0000000 | 0.7550000 | S |
	| 0.7550000 | 2.5650000 | V |
	| 2.5650000 | 3.4550000 | S |
	| 3.4550000 | 4.7350000 | V |
	| 4.7350000 | 6.0075000 | S |
	| 6.0075000 | 7.5525000 | V |
	| 7.5525000 | 9.0375000 | S |	

	Si lo comparamos con el resultado obtenido, vemos que nuestro sistema (Audio2.vad) no funciona especialmente bien con este archivo. Se debe a que la voz suena con muy poca potencia en relación al silencio. Sin embargo, como hemos optimizado el F-Score total , no es tan relevante. Puesto que en los otros casos nos da muy buen resultado: 94,201 %

- A la vista de la gráfica, indique qué valores considera adecuados para las magnitudes siguientes:

	* Incremento del nivel potencia en dB, respecto al nivel correspondiente al silencio inicial, para
	  estar seguros de que un segmento de señal se corresponde con voz.
	  
		Si hay un incremento de 27dB podemos decir que empieza un segemnto de voz.

	* Duración mínima razonable de los segmentos de voz y silencio.
		
		Duración mínima del silencio podemos decir que es 0,7s y de voz 1,2s. (Muy lejos de lo que observamos con otros audios, por lo que en el programa utilizamos otros parámetros) Al ir ajustando los parámetros observamos que finalmente los tiempos a considerar son los siguientes:

		```c
		if(time_passed>0.266 || (time_passed>0.204 && f.p<vad_data->umbral1)){...} //Caso de MYB_SILENCE A SILENCE

		if((time_passed>0.05 && f.p>vad_data->alpha2) || time_passed >0.08){...} //caso de MYB_VOICE A VOICE
		```
		Como se observa  (y comentaremos más a detalle adelante en adaptaciones del algoritmo) usamos distintos tiempos. En el caso de voz, consideramos que 0.08 es un tiempo considerable (nos optimiza la F-Score) y 0.05 en caso de que supere el segundo umbral (y más restrictivo). Lo mismo con Silencio pero con números mayores, pues es donde somos más restrictivos.


	* ¿Es capaz de sacar alguna conclusión a partir de la evolución de la tasa de cruces por cero?
		
		La tasa de cruces por cero nos puede ayudar a diferenciar cuando se trata de silencio o de una consonante fricativa, por ejemplo. Esto es, ya que estas tienen poca potencia y podrían llegar a considerarse silencio. Sin embargo, en nuestro algoritmo no lo hemos usado pues no ayudaba a mejorar las estadísticas.

### Desarrollo del detector de actividad vocal

- Complete el código de los ficheros de la práctica para implementar un detector de actividad vocal en
  tiempo real tan exacto como sea posible. Tome como objetivo la maximización de la puntuación-F `TOTAL`.
	
	Tras completar el código y optimizar tanto los valores de los umbrales como de las codiciones de transición (Mediante tiempo y potencia) obtenemos una F-Score de
	> TOTAL: 94.203%

- Inserte una gráfica en la que se vea con claridad la señal temporal, el etiquetado manual y la detección
  automática conseguida para el fichero grabado al efecto. 


	Aprovechando que hemos hecho el archivo wav de salida, adjuntamos la gráfica de dos señales de voz de la base de datos (Una original y otra con el vad) y se verá de forma más clara:
	
    ![Alt text](./WaveSurfer_Graphs-1.png?raw=true "Optional Title")
	
	
	Se ha realizado con el fichero de audio pav_4305.wav. Se puede consultar el .vad en prueba4305.vad y el fichero de audio de salida en pruebapp2.wav. El contenido del archivo .vad en comparacion con el teórico es:

	![Alt text](./comp.png?raw=true "Optional Title")

- Explique, si existen. las discrepancias entre el etiquetado manual y la detección automática.

	Analizando los resultados de Recall y Precisión vemos que la peor es el recall del Silencio con 87,6% y el mejor el recall de la voz con un 96.38%. Esto indica que hemos considerado tramas de silencio como voz, maximizando la detección de voz para no perder información. Consideramos que caracterizar tramas de voz como silencio es más perjudicial que considerar tramas de silencio como voz, ya que la información se preserva.



- Evalúe los resultados sobre la base de datos `db.v4` con el script `vad_evaluation.pl` e inserte a 
  continuación las tasas de sensibilidad (*recall*) y precisión para el conjunto de la base de datos (sólo
  el resumen).

 ![Alt text](./f-score.png?raw=true "Optional Title")




### Trabajos de ampliación

#### Cancelación del ruido en los segmentos de silencio

- Si ha desarrollado el algoritmo para la cancelación de los segmentos de silencio, inserte una gráfica en
  la que se vea con claridad la señal antes y después de la cancelación (puede que `wavesurfer` no sea la
  mejor opción para esto, ya que no es capaz de visualizar varias señales al mismo tiempo).
  


  ![Alt text](./WaveSurfer_Graphs-1.png?raw=true "Optional Title")

	Para poder llevar a cabo este ejercicio de ampliación, se ha tenido que seguir un procedimiento (se puede ver más en profundidad desde el código) donde al leer el buffer, este se escribía en el archivo .wav de salida. de forma que:
	```c
	if (sndfile_out != 0) {
		sf_write_float(sndfile_out, buffer, frame_size); //We write into the output file the samples of the buffer
		}

	```
	Una vez pasado los valores de la señal original al de salida, es necesario saber cuándo se debe poner a 0 las X muestras correspondientes. Para ello ay disponíamos de los tiempos defined_t y last_t que definen las tramas en un mismo estado definido (restadas). Por ello, al ver que estamos en un estado de silencio confirmado, ponemos a 0 todas estas tramas. Es interesante comentar que la función write_float de la librería sndfile.h admite un máximo valor en el t_count, por lo que ha sido necesario dividir la escritura de 0 en escrituras más pequeñas de valor 161*frame_size. Aproximadamente 25.000.
	```c
	if (sndfile_out != 0 && defined_state == ST_SILENCE) { //if there is output file and the last defined state is Silence (which means we have just switch to voice) we put to 0 all the samples of silence.
              sf_seek(sndfile_out, defined_t* frame_size, SEEK_SET); //Point to the position of the file where the Silence starts (defined_t * frame_size)
              int frames = (last_t-1-defined_t)*frame_size; //Difference of frames in which the state has taken place
              
              
              while(frames >= 161 * frame_size){ //we have to divide the writes because the function does not support numbers larger than approx n ~= 161*frame

                sf_write_float(sndfile_out,buffer_zeros, 161* frame_size);
                frames -= 161*frame_size;
              }

              sf_write_float(sndfile_out,buffer_zeros, frames); // last write. If frames was inittialy < (161 * frames_size) it will also be the only write
                                                               // It is actually the normal thing. But we have to consider it for large silence frames.
              sf_seek(sndfile_out, 0, SEEK_END); // point to the end of the file for the next iteration to write the buffer

	```

 

#### Gestión de las opciones del programa usando `docopt_c`

- Si ha usado `docopt_c` para realizar la gestión de las opciones y argumentos del programa `vad`, inserte
  una captura de pantalla en la que se vea el mensaje de ayuda del programa.

Hecho. El Help funcionaría: 


![Alt text](./help.png?raw=true "Optional Title")

### Contribuciones adicionales y/o comentarios acerca de la práctica

- Indique a continuación si ha realizado algún tipo de aportación suplementaria (algoritmos de detección o 
  parámetros alternativos, etc.).

	Hemos usado dos umbrales de detección para poder afinar más con la detección. Además, hemos sido más restrictivos a la hora de pasar de maybeSilence a silence que de maybeVoice a voice. Esto para maximizar la detección de voz, pues no puede perderse información. Para ello hemos usado tanto los umbrales como el tiempo en los estados (calculado a partir del número de tramas). 
	También hemos tenido en cuenta, a la hora de calcular los umbrales, la potencia logarítmica de las primeras 15 tramas. Creemos que esta es la medida más fiable a la hora de contemplar el ruido.
	$P_{dB} = 10 \log_{10} (\dfrac{1}{Nint} · \sum_{n = 0}^{Nint-1 }10^{{Pdb[i]}/{10}}[{n}] )$;


	Para ello
	```c
	if(nint==15){
		vad_data->umbral1 = 10*log10((vad_data->umbral1)/nint) + vad_data->alpha1;
		//fprintf(stdout, "%f", vad_data->umbral1);
		vad_data->umbral2 = vad_data->umbral1 + vad_data->alpha2;
		vad_data->state = ST_SILENCE;
		nint =0;
	}
	else{
		
		vad_data->umbral1 += pow(10, (f.p/10) );
		//fprintf(stdout, "%f", vad_data->umbral1);
		nint++;
		}
	```
	En cuanto, al uso de estados de transición y tiempos hemos decidido lo siguiente:

	```c
		case ST_SILENCE:
		if (f.p > vad_data->umbral1)
		vad_data->state = ST_MYB_VOICE; /*If Power of frame is bigger than Treshold1 we enter MYBV state*/
		break;

	case ST_VOICE:
		if (f.p < vad_data->umbral2)
		vad_data->state = ST_MYB_SILENCE; /*If power is lower than Tresholdl2 we enter MYBS state*/
		break;

		case ST_MYB_SILENCE: 
			if(f.p < vad_data->umbral2){  
			if(time_passed>0.266 || (time_passed>0.204 && f.p<vad_data->umbral1)){  /*We use 2 conditions to change to Silence state beacause 
																						we want to be very sure we are in Silence state and not lose info.*/
				vad_data->state = ST_SILENCE;
				vad_data->maybe_count= 0; /*restart the Maybe counter*/
			}
			else {
				vad_data->maybe_count ++;  /*If conditions are not passed we add 1 to the count of frames in a Maybe state */
			}
			}
			else{
			vad_data->state = ST_VOICE; /*If conditions above are not passed we can say we are in Voise state*/
			vad_data->maybe_count= 0;  /*restart the Maybe counter*/

			}


		break;

		case ST_MYB_VOICE:
			if(f.p > vad_data->umbral1){
			if((time_passed>0.05 && f.p>vad_data->alpha2) || time_passed >0.08){ /*We are less restrictive to enter Voice state to not lose information*/
				vad_data->state = ST_VOICE;
				vad_data->maybe_count= 0;  /*restart the Maybe counter*/
			}
			else {
				vad_data->maybe_count ++; /*If conditions are not passed we add 1 to the count of frames in a Maybe state */
			}
			}
			else{
			vad_data->state = ST_SILENCE;
			vad_data->maybe_count= 0;  /*restart the Maybe counter*/

			}
	```

	Como se observa, en el caso de voz, consideramos que 0.08 es un tiempo considerable (nos optimiza la F-Score) y 0.05 en caso de que supere el segundo umbral (y más restrictivo por lo que el tiempo puede ser menor). Lo mismo con Silencio pero con números mayores, pues es donde somos más restrictivos. Para contar el tiempo que lleva se utiliza una variable de la propia estrucutra vad_data wue hemos denominado maybe_count;

	```c
		float time_passed = FRAME_TIME * 1e-3 * vad_data->maybe_count;

	```

	Es decir, el algoritmo primero mira el umbral menos restrictivo (En caso de los MAYBE) y si lo cumple ocurren dos cosas:
				
	>  1.	Si no supera el umbral más restrictivo, tiene en cuenta un tiempo de transición mayor. Puesto que se si se encuentra entre estos dos umbrales un largo tiempo debe cambiar.
	
	>  2.   Si supera el umbral más restrictivo el tiempo a considerar es menor. No es nulo para evitar silencios muy cortos entre fonemas o ruidos muy cortos como golpes en el micro, de fondo etc...



	Cabe comentar que se debe hacer un tratamiento especialm para la última trama. Si el estado de esta última está definido (SILENCE o VOICE) el tratamientto es el mimso que normal (Teniendo en cuenta los n_read). Sin embargo, si está en un estado indecidido, se debe establecer cuál es en base al úiltimo::

	```c
	state = vad_close(vad_data);

   if (sndfile_out != 0) {
      sf_write_float(sndfile_out, buffer, n_read); //We write into the output file the samples of the last buffer
    }
   

  /* TODO: what do you want to print, for last frames? */
  if (t != last_t){
    if(state==ST_VOICE || state== ST_SILENCE){ //If the current state is Defined, We do the same as in the previous loop but with the last frame

      fprintf(vadfile, "%.5f\t%.5f\t%s\n", defined_t * frame_duration, t * frame_duration + n_read / (float) sf_info.samplerate, state2str(state));
      
      if (sndfile_out != 0 && state == ST_SILENCE) {
              sf_seek(sndfile_out, defined_t* frame_size  , SEEK_SET); // Point to the defined_t
              
              int frames = (t-defined_t) * frame_size + n_read; //We need to consider the n_read samples of last frame
              
            
              while(frames >= 161 * frame_size){ //we have to divide the writes because the function does not support numbers larger than approx 161*frame

                sf_write_float(sndfile_out,buffer_zeros, 161* frame_size);
                frames -= 161*frame_size;
              }

              sf_write_float(sndfile_out,buffer_zeros, frames); // last write. If frames was inittialy < 161 * frames_size also the only write
              
               
              //Here there is no need to update the pointer as it is the last frame
      }
    }

    else{ //if it is in an undefined_state (MYBE) we have to consider the output
      if(defined_state == ST_SILENCE){
        state = ST_SILENCE; //if the last defined state was silence, it will be silence
         if (sndfile_out != 0) {
              sf_seek(sndfile_out, defined_t* frame_size, SEEK_SET);
             int frames = (t-defined_t) * frame_size + n_read; //We need to consider the n_read samples of last frame
              
            
              while(frames >= 161 * frame_size){ //we have to divide the writes because the function does not support numbers larger than approx 161*frame

                sf_write_float(sndfile_out,buffer_zeros, 161* frame_size);
                frames -= 161*frame_size;
              }

              sf_write_float(sndfile_out,buffer_zeros, frames); 
      }  }
        else state = ST_VOICE; // if the defined_state was voice, it will be voice. But normally the end of files appear to be silence
     fprintf(vadfile, "%.5f\t%.5f\t%s\n", defined_t * frame_duration, t * frame_duration + n_read / (float) sf_info.samplerate, state2str(state));
      }
    
   
  }

	```



- Si lo desea, puede realizar también algún comentario acerca de la realización de la práctica que
  considere de interés de cara a su evaluación.


### Antes de entregar la práctica

Recuerde comprobar que el repositorio cuenta con los códigos correctos y en condiciones de ser 
correctamente compilados con la orden `meson bin; ninja -C bin`. El programa generado (`bin/vad`) será
el usado, sin más opciones, para realizar la evaluación *ciega* del sistema.


### Hecho