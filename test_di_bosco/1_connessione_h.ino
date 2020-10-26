// 1_connessione_h: intestazione del modulo di connessione bluetooth al terminale

#ifndef CONNESSIONE_H
#define CONNESSIONE_H

#include <SoftwareSerial.h>

namespace Connessione {

	class Terminale {
	public:
		// inizializzazione obbligatoria
		Terminale(Apparecchiatura::Barriera& barriera, int pin_tra, int pin_ric,
				  long baud = baud_def, HardwareSerial& debug = Serial, long baud_debug = baud_debug_def);
		Terminale(Apparecchiatura::Barriera& barriera, int pin_tra, int pin_ric,
				  HardwareSerial& debug, long baud_debug = baud_debug_def);
		Terminale() = delete;
		// spostamento consentito
		Terminale(Terminale&&) = default;
		Terminale& operator=(Terminale&&) = default;
		// copia vietata: un pin dovrebbe appartenere a un solo terminale
		Terminale(const Terminale&) = default;
		Terminale& operator=(const Terminale&) = delete;
		// distruttore predefinito
		~Terminale() = default;

		// costanti
		static constexpr long baud_def { 9600 }, baud_debug_def { 9600 };
		static constexpr char comando_stato { '0' }, // chiede se la barriera sia accesa
							  comando_spegni { '1' }, // spegne la barriera
							  comando_accendi { '2' }, // accende la barriera
							  comando_chiedi { '3' }, // chiede una prova di N salti
							  comando_abortisci { '4' }; // annulla la richiesta
		static constexpr int salti_richiesti_default { 1 };
		static constexpr size_t salti_richiesti_string_dim { 10 };

		// getter
		Apparecchiatura::Barriera& barriera() const;
		int pin_tra() const;
		int pin_ric() const;
		long baud() const;
		HardwareSerial& debug() const;
		long baud_debug() const;

		void setup();
		void loop();

	private:
		Apparecchiatura::Barriera& bar; // la barriera controllata dal terminale
		int pin_tx, pin_rx; // pin del flusso di comunicazione col terminale
		SoftwareSerial m; // modulo per la comunicazione col terminale
		long b; // velocità di trasmissione (in baud) col terminale
		HardwareSerial& d; // pin del flusso di comunicazione per debug
		long b_d; // velocità di trasmissione (in baud) per debug 

		void azione_stato();
		void azione_spegni();
		void azione_accendi();
		void azione_chiedi();
		bool azione_aborto();
		void azione_sconosciuta(char input);

		int calcola_salti_richiesti();
	};
	
}

#endif
