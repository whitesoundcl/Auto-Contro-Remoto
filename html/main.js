const DIRECCION_CENTRADA = 127;

var sliderDireccion = document.getElementById("sliderDireccion");
var sliderPotencia = document.getElementById("sliderPotencia");
var btnCentrarDireccion = document.getElementById("btnCentrarDireccion");
var lblValorDireccionActual = document.getElementById("lblValorDireccionActual");
var lblStatusAjax = document.getElementById("lblStatusAjax");
var lblPotencia = document.getElementById("lblPotencia");
var lblConexion = document.getElementById("lblConexion");
var txtDirMax = document.getElementById("txtDirMax");
var txtDirMin = document.getElementById("txtDirMin");
var btnGuardarEEPROM = document.getElementById("btnGuardarEEPROM");
var txtControlar = document.getElementById("txtControlar");
var teclas = [];
var direccionMaxIzq = 0
var direccionMaxDer = 255;
var potencia = 0;
var direccionIP = "192.168.1.131"; // Reemplaza tu dir. ip.

enviarPeticionAjax([{
    ruta: "configuracion",
    parametro: "datos", valor: 0
}], (respuesta)=>{
    let datos = JSON.parse(respuesta);
    txtDirMax.value = datos.max_dir;
    txtDirMin.value = datos.min_dir;
    lblConexion.innerHTML = "Conexión establecida"
});


lblValorDireccionActual.innerHTML = sliderDireccion.value; 

sliderDireccion.oninput = function () {
    lblValorDireccionActual.innerHTML = this.value;
    //actualizarDireccion(this.value);
}

sliderPotencia.oninput = function() {
    potencia = this.value;
    lblPotencia.innerHTML = this.value;
}
// Se realiza la llamada ajax al microcontrolador
sliderDireccion.onmouseup = function () {
    actualizarDireccion(this.value);
}

btnCentrarDireccion.onclick = function () {
    sliderDireccion.value = DIRECCION_CENTRADA;
    actualizarDireccion(DIRECCION_CENTRADA);
}

btnGuardarEEPROM.onclick = function() {
    enviarPeticionAjax([{
        ruta: "configuracion",
        parametro: "guardar", valor: "si"
    }], (respuesta)=>{});
}

txtDirMax.onkeypress = function(e) {
    if(e.key == "Enter") {
        enviarPeticionAjax([{
            ruta: "configuracion",
            parametro: "max_dir", valor: this.value
        }], (respuesta)=>{});
        
    }
}

txtDirMin.onkeypress = function(e) {

    if(e.key == "Enter") {
        enviarPeticionAjax([{
            ruta: "configuracion",
            parametro: "min_dir", valor: this.value
        }], (respuesta)=>{});
        
    }
}

txtControlar.onkeydown = function(e) {
    e.preventDefault();

    //Envia la peticion una vez por tecla presionada.
    if(!teclas[e.key]) {
        teclas[e.key] = true;
        console.log(e.key);
        if(e.key == "ArrowRight") {
            actualizarDireccion(direccionMaxDer);
        } else if (e.key == "ArrowLeft"){
            actualizarDireccion(direccionMaxIzq);
        } else if (e.key == "ArrowUp"){
            avanzar(potencia);
        }
    }
    
    
}

txtControlar.onkeyup = function(e) {
    e.preventDefault();

    teclas[e.key] = false;
    if(!teclas["ArrowRight"] && !teclas["ArrowLeft"]){
        actualizarDireccion(DIRECCION_CENTRADA);
    }
    if(e.key == "ArrowUp"){
        avanzar(0);
    }
    // baja potencia
    if (e.key == "x") {
        cambiarPotencia(55);
    }
    if(e.key == "z"){
        cambiarPotencia(60);
    }
    // Mediana potencia
    if(e.key == "a") {
        cambiarPotencia(130);
    }
    // Toda la potencia
    if(e.key == "q"){
        cambiarPotencia(160);
    }

}

function actualizarDireccion(valor) {
    enviarPeticionAjax([{
        ruta: "controlar",
        parametro: "dir", valor: valor
    }],(respuesta)=>{
        lblStatusAjax.innerHTML = respuesta;
    });
}

function avanzar(valor){
    enviarPeticionAjax([{
        ruta: "controlar",
        parametro: "motor", valor: valor
    }],(respuesta)=>{

    })
}

function cambiarPotencia(pot){
    potencia = pot;
    lblPotencia.innerHTML = pot;
    sliderPotencia.value = pot;
}

function enviarPeticionAjax(peticiones, callback) {
    console.log("AJAX ");
    console.log(peticiones);
    
    
    peticiones.forEach(peticion => {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.status != 200) {
                //lblStatusAjax.innerHTML = this.status;
            }
            if (this.readyState == 4 && this.status == 200) {
                callback(this.responseText)
                //lblStatusAjax.innerHTML = this.responseText;
            }
        };
        xhttp.open("GET", "http://"+direccionIP+"/" + peticion.ruta + "?" + peticion.parametro + "=" + peticion.valor, true);
        xhttp.send();
    });
}
