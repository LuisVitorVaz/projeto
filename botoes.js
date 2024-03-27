import {circulosAdicionados, trajetoria,alertCircleRed,toggleCircle,barcoprimeiro} from './configmapa.js'; // Importa as funções necessárias

let barco1State = false; // Variável para armazenar o estado do botão barco1
let isRoute1Clicked = false;
let isRoute2Clicked = false;
let isRoute3Clicked = false;
let isRoute4Clicked = false;
let isRoute5Clicked = false;
let isRoute6Clicked = false;
let isRoute1Visible = true; // Variável para controlar a visibilidade da rota 1
let isRoute4Visible = true;
export function barco1() {
    barco1State = !barco1State; // Inverte o estado do botão barco1

    // Alterna a cor do botão entre azul e a cor original
    if (barco1State) {
        document.getElementById('boat1-btn').style.backgroundColor = '#ADD8E6'; // Muda cor para azul claro
       
    } else {
        document.getElementById('boat1-btn').style.backgroundColor = ''; // Restaura a cor original
       
    }
}
// BOTAO VERDE COM O TITULO PONTOS
export function handleRoute1Click() {
   
    isRoute1Clicked = !isRoute1Clicked;
    
    // Alterna a cor do botão entre amarelo e a cor original
    if (isRoute1Clicked) {
      document.getElementById('route1-btn').style.backgroundColor = 'yellow'; // Muda cor para amarelo
    } else {
      document.getElementById('route1-btn').style.backgroundColor = ''; // Restaura a cor original
    }
    
    // Alternar a visibilidade da rota no mapa
    if (isRoute1Visible) {
        ocultarCirculo(circulosAdicionados); // Oculta o círculo
    } else {
        mostrarCirculo(circulosAdicionados); // Mostra o círculo
    }
    
    isRoute1Visible = !isRoute1Visible; // Inverte o estado de visibilidade da rota 1
}

// BOTAO AZUL COM O TITULO TRACADO
export function handleRoute2Click() {
    isRoute2Clicked = !isRoute2Clicked; // Inverte o estado de clique
    if (isRoute2Clicked) {
        document.getElementById('route2-btn').style.backgroundColor = 'yellow'; // Muda cor para amarelo

        trajetoria.forEach(circulo => {
            circulo.setStyle({ fillOpacity: 0, color: 'transparent' }); // Oculta cada ponto na lista
        });
    } else {
        document.getElementById('route2-btn').style.backgroundColor = ''; // Restaura a cor original

        trajetoria.forEach(circulo => {
            circulo.setStyle({ fillOpacity: 0.5, color: 'blue' }); // Restaura a opacidade e a cor do círculo
        });
    }
}
// BOTAO DE ALERTA 

export function handleRoute3Click() {
    isRoute3Clicked = !isRoute3Clicked; // Inverte o estado de clique
    
    if (isRoute3Clicked) {
        document.getElementById('route3-btn').style.backgroundColor = 'hsl(0, 70%, 80%)'; // Muda cor para vermelho o botao
       
        alertCircleRed.setStyle({ fillOpacity: 0, fillColor: 'transparent', color: 'transparent' });
        }

     else {
        document.getElementById('route3-btn').style.backgroundColor = ''; // Restaura a cor original
        
            alertCircleRed.setStyle({ fillOpacity: 0.5, color: 'red', fillColor: 'red'}); // Restaura a opacidade e a cor do círculo
        toggleCircle(true);
        // Não é necessário chamar toggleCircle aqui
    }
}
// Handler Clicks Barco 2

// BOTAO VERDE COM O TITULO PONTOS
export function handleRoute4Click() {
    isRoute4Clicked = !isRoute4Clicked;
    
    // Alterna a cor do botão entre amarelo e a cor original
    if (isRoute4Clicked) {
        document.getElementById('route4-btn').style.backgroundColor = 'yellow'; // Muda cor para amarelo
    } else {
        document.getElementById('route4-btn').style.backgroundColor = ''; // Restaura a cor original
    }
    
    // Alternar a visibilidade da rota no mapa
    if (isRoute4Visible) {
        // ocultarCirculo(circulosAdicionados2); // Oculta o círculo
    } else {
        // mostrarCirculo(circulosAdicionados2); // Mostra o círculo
    }
    
    isRoute4Visible = !isRoute4Visible; // Inverte o estado de visibilidade da rota 1
}

// BOTAO AZUL COM O TITULO TRACADO
export function handleRoute5Click() {
    isRoute5Clicked = !isRoute5Clicked; // Inverte o estado de clique
    if (isRoute5Clicked) {
        document.getElementById('route5-btn').style.backgroundColor = 'yellow'; // Muda cor para amarelo

        // trajetoria2.forEach(circulo => {
        //     circulo.setStyle({ fillOpacity: 0, color: 'transparent' }); // Oculta cada ponto na lista
        // });
    } else {
        document.getElementById('route5-btn').style.backgroundColor = ''; // Restaura a cor original

        // trajetoria2.forEach(circulo => {
        //     circulo.setStyle({ fillOpacity: 0.5, color: 'blue' }); // Restaura a opacidade e a cor do círculo
        // });
    }
}

// BOTAO DE ALERTA 
export function handleRoute6Click() {
    isRoute6Clicked = !isRoute6Clicked; // Inverte o estado de clique
    
    if (isRoute6Clicked) {
        document.getElementById('route6-btn').style.backgroundColor = 'hsl(0, 70%, 80%)'; // Muda cor para vermelho o botao
       
        // alertCircleRed2.setStyle({ fillOpacity: 0, fillColor: 'transparent', color: 'transparent' });
    } else {
        document.getElementById('route6-btn').style.backgroundColor = ''; // Restaura a cor original
        
        // alertCircleRed2.setStyle({ fillOpacity: 0.5, color: 'red', fillColor: 'red'}); // Restaura a opacidade e a cor do círculo
    }
}
// Dentro de ocultarCirculo e mostrarCirculo, você chama toggleCircle com a variável 
function  ocultarCirculo(circulos) {
    console.log("ocultando circulo");
    circulos.forEach(circulo => {
        circulo.setStyle({ fillOpacity: 0,color: 'transparent' }); // Oculta cada ponto na lista
    });
}
function mostrarCirculo(circulos) {
    console.log("mostrar circulo");
    circulos.forEach(circulo => {
        circulo.setStyle({ fillOpacity: 0.5 }); // Mostra cada ponto na lista
    });
}

document.getElementById('boat1-btn').addEventListener('click', barco1);
document.getElementById('route1-btn').addEventListener('click', handleRoute1Click);
document.getElementById('route2-btn').addEventListener('click', handleRoute2Click);
document.getElementById('route3-btn').addEventListener('click', handleRoute3Click);
document.getElementById('route4-btn').addEventListener('click', handleRoute4Click);
document.getElementById('route5-btn').addEventListener('click', handleRoute5Click);
document.getElementById('route6-btn').addEventListener('click', handleRoute6Click);


