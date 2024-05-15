int maximumRange = 200; // Alcance máximo necessário
int minimumRange = 0; // Alcance mínimo necessário
long duration, distance; // Duração usada para calcular a distância

// Definição dos pinos de Ultrassom
const int echoPin = 11;
const int trigPin = 12;

// Definição dos pinos dos LEDs para a Rua Principal
const int ruaPrincipalVerde1 = 2;
const int ruaPrincipalAmarelo1 = 3;
const int ruaPrincipalVermelho1 = 4;
const int ruaPrincipalPedestreVerde = 6;
const int ruaPrincipalPedestreVermelho = 7;

// Definição dos pinos dos LEDs para a Rua Secundária
const int ruaSecundariaVerde = 8;
const int ruaSecundariaAmarelo = 9;
const int ruaSecundariaVermelho = 10;
const int ruaSecundariaWarning = A0;

// Definição do pino do botão da Rua Principal
const int ruaPrincipalBotao = 13;

// Definição do pino do buzzer para os semáforos dos peões
const int buzzerPin = 5;

// Variáveis para controlar a luz amarela a piscar na rua secundária
bool luzAmarelaPiscar = false;
unsigned long tempoInicioPiscar = 0;
const unsigned long intervaloPiscar = 500; // Intervalo de piscar em milissegundos

void inicializarSemaforos() {
  // Define o estado inicial dos semáforos
  digitalWrite(ruaSecundariaVerde, LOW);
  digitalWrite(ruaSecundariaAmarelo, LOW);
  digitalWrite(ruaSecundariaVermelho, HIGH);

  digitalWrite(ruaPrincipalVerde1, HIGH);
  digitalWrite(ruaPrincipalVermelho1, LOW);
  digitalWrite(ruaPrincipalAmarelo1, LOW);

  digitalWrite(ruaPrincipalPedestreVerde, LOW);
  digitalWrite(ruaPrincipalPedestreVermelho, HIGH);
}

void acionarSemaforosPedestres() {
  digitalWrite(ruaPrincipalVerde1, LOW);
  digitalWrite(ruaPrincipalAmarelo1, HIGH);
  digitalWrite(ruaPrincipalVermelho1, LOW);
  delay(2000);

  // Desliga os semáforos dos veículos
  digitalWrite(ruaPrincipalVerde1, LOW);
  digitalWrite(ruaPrincipalAmarelo1, LOW);
  digitalWrite(ruaPrincipalVermelho1, HIGH);

  // Lógica para acionar os semáforos de pedestres
  digitalWrite(ruaPrincipalPedestreVerde, HIGH);
  digitalWrite(ruaPrincipalPedestreVermelho, LOW);

  // Lógica para acionar os semáforos da rua Secundária
  digitalWrite(ruaSecundariaVerde, HIGH);
  digitalWrite(ruaSecundariaAmarelo, LOW);
  digitalWrite(ruaSecundariaVermelho, LOW);
  delay(1000);
}

void warningAmareloSecundaria() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ruaSecundariaWarning, HIGH);
    delay(500);
    digitalWrite(ruaSecundariaWarning, LOW);
    delay(500);
  }
}

void avisoFimVerdePedestres() {
  // Alerta sonoro nos últimos 3 segundos
  for (int i = 3; i > 0; i--) {
    // Aguarda 1 segundo
    if (i <= 3 && i >= 1) {
      // Ativa o buzzer por 1 segundo
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(ruaPrincipalPedestreVerde, HIGH);
      digitalWrite(ruaSecundariaWarning, HIGH);
      delay(500);
      digitalWrite(buzzerPin, LOW);
      digitalWrite(ruaPrincipalPedestreVerde, LOW);
      digitalWrite(ruaSecundariaWarning, LOW);
      delay(500);
    }
  }
}

void reiniciarSemaforosVeiculos() {
  // Lógica para reiniciar os semáforos dos veículos após os pedestres atravessarem
  digitalWrite(ruaPrincipalPedestreVerde, LOW);
  digitalWrite(ruaPrincipalPedestreVermelho, HIGH);

  // Lógica para reiniciar os semáforos dos veículos da rua secundária após travessia
  digitalWrite(ruaSecundariaVerde, LOW);
  digitalWrite(ruaSecundariaAmarelo, HIGH);
  digitalWrite(ruaSecundariaVermelho, LOW);
  delay(2000);
  digitalWrite(ruaSecundariaVerde, LOW);
  digitalWrite(ruaSecundariaAmarelo, LOW);
  digitalWrite(ruaSecundariaVermelho, HIGH);

  // Liga os semáforos dos veículos
  digitalWrite(ruaPrincipalVerde1, HIGH);
  digitalWrite(ruaPrincipalVermelho1, LOW);
}

// Refatoração da lógica de distância do ultrassom em uma função separada
int distanciaUltrassom() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  Serial.println(distance);
  return distance;
}

void setup() {
  // Configuração dos pinos dos LEDs da Rua Principal como saída
  pinMode(ruaPrincipalVerde1, OUTPUT);
  pinMode(ruaPrincipalVermelho1, OUTPUT);
  pinMode(ruaPrincipalAmarelo1, OUTPUT);
  pinMode(ruaPrincipalPedestreVerde, OUTPUT);
  pinMode(ruaPrincipalPedestreVermelho, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Configuração dos pinos dos LEDs da Rua Secundária como saída
  pinMode(ruaSecundariaVerde, OUTPUT);
  pinMode(ruaSecundariaAmarelo, OUTPUT);
  pinMode(ruaSecundariaVermelho, OUTPUT);
  pinMode(ruaSecundariaWarning, OUTPUT);

  pinMode(ruaPrincipalBotao, INPUT);

  // Configuração dos pinos trig e echo do Ultrassom
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  inicializarSemaforos();
}

void loop() {
  // Lógica para o botão de pedestres na Rua Principal
  if (digitalRead(ruaPrincipalBotao) == HIGH) {
    acionarSemaforosPedestres();
    warningAmareloSecundaria();
    avisoFimVerdePedestres();
    reiniciarSemaforosVeiculos();
  }

  int dist = distanciaUltrassom();
  if (dist < 5) {
    acionarSemaforosPedestres();
    int aux = 0;
    while (aux <= 3) {
      warningAmareloSecundaria();
      dist = distanciaUltrassom();
      if (dist < 5) {
        aux++;
        Serial.println(aux);
      } else {
        break;
      }
    }
    avisoFimVerdePedestres();
    reiniciarSemaforosVeiculos();
    delay(10000);
  }

  // Lógica para controlar a luz amarela piscar
  static bool luzAmarelaPiscar = false;
  static unsigned long tempoInicioPiscar = 0;

  bool semaforoVerdeVeiculosRuaSecundaria = digitalRead(ruaSecundariaVerde) == HIGH;
  bool semaforoVerdePeoesRuaPrincipal = digitalRead(ruaPrincipalPedestreVerde) == HIGH;

  if (semaforoVerdeVeiculosRuaSecundaria && semaforoVerdePeoesRuaPrincipal) {
    if (!luzAmarelaPiscar) {
      luzAmarelaPiscar = true;
      tempoInicioPiscar = millis(); // Guarda o tempo atual para controlar a piscagem
    }

    // Lógica para piscar a luz amarela da rua secundária
    unsigned long tempoAtual = millis();
    if (tempoAtual - tempoInicioPiscar >= intervaloPiscar) {
      digitalWrite(ruaSecundariaWarning, !digitalRead(ruaSecundariaWarning)); // Inverte o estado da luz amarela
      tempoInicioPiscar = tempoAtual; // Atualiza o tempo de início para o próximo intervalo
    }
  } else {
    // Se os semáforos não estiverem ambos verdes, desativa o piscar da luz amarela
    luzAmarelaPiscar = false;
    digitalWrite(ruaSecundariaWarning, LOW); // Garante que a luz amarela pare de piscar quando não necessário
  }
  delay(50);
}
