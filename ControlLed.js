var ControlLed = function(id){
	var ui = $("#plantillas .control_led").clone();
	var boton = ui.find(".boton_enviar");
	var led = ui.find(".led");
	led.attr("id", "led_" + id);
	boton.bind('touchstart', function(){
		var mensaje = JSON.stringify({
				estadoBoton:"presionado",
				idNodo:id
			})+'|';
		serial.write(
			mensaje,
			function(successMessage) {
				console.log("enviado: ", mensaje);
			},
			function(err){
				console.log("error al enviar por puerto serie:", err);
			}
		);	
	});

	boton.bind('touchend', function(){
		var mensaje = JSON.stringify({
				estadoBoton:"suelto",
				idNodo:id
			})+'|';
		serial.write(
			mensaje,
			function(successMessage) {
				console.log("enviado: ", mensaje);
			},
			function(err){
				console.log("error al enviar por puerto serie:", err);
			}
		);	
	});
	
	
	$("#panel_de_mando").append(ui);
};
