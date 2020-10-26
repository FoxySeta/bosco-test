// 2_main_cpp: il punto di ingresso del programma (contiene setup() e void())

namespace {
	constexpr int pin_laser_inizio { 2 },
				  pin_laser_fine { 3 },
				  pin_fotoresistenza_inizio { 0 },
				  pin_fotoresistenza_fine { 1 },
				  pin_bluetooth_trasmettitore { 10 },
				  pin_bluetooth_ricevitore { 11 };
	
	Apparecchiatura::Barriera pedana(pin_laser_inizio, pin_laser_fine, pin_fotoresistenza_inizio, pin_fotoresistenza_fine);
	Connessione::Terminale operatore(pedana, pin_bluetooth_trasmettitore, pin_bluetooth_ricevitore); 
}

void setup() {
	operatore.setup();
}

void loop() {
	operatore.loop();
}
