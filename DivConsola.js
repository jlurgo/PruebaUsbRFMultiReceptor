var DivConsola = {
	start: function(){
		var _this = this;
		this.divConsola = $("<div id='div_consola'>");
		this.divConsola.hide();
		$("body").append(this.divConsola);
		
		var oldLog = console.log;
		console.log = function(){
			var div_entrada = $("<div>");
			_.forEach(arguments, function(arg){
				if(_.isString(arg)) div_entrada.text(div_entrada.text() + arg + " ");
				else {
					if(_.isObject(arg)){
						try{
							div_entrada.text(div_entrada.text() + JSON.stringify(arg));
						} catch(err){
							div_entrada.text(div_entrada.text() + "imposible serializar objeto");
						}
					}
					else 
						div_entrada.text(div_entrada.text() + arg.toString())
				};
			});
			_this.divConsola.append(div_entrada);
			oldLog.apply(console, arguments);
		};		
		
		this.btnConsola = $("<input id='btn_consola' type='button' value='/'>");
		$("body").append(this.btnConsola);
		
		this.btnConsola.click(function(){
			_this.divConsola.toggle();
		});
	}	
};