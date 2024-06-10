//#include <NonBlockingRtttl.h>
//
////project's contants
//#define BUZZER_PIN 4
//const char * firealarm = "firealarm:d=4,o=6,b=200:c,c,c,p,c,c,c,p,c,c,c,p,c,c,c,p";
//void setup() {
//  pinMode(BUZZER_PIN, OUTPUT);
//  Serial.println();
//}
//
//void loop() {
//  rtttl::begin(BUZZER_PIN, firealarm);
//  while( !rtttl::done() ) 
//  {
//    rtttl::play();
//  } 
//}

//const int buzzerPin = 4;  // Chân kết nối với buzzer
//const char * mario = "mario:d=4,o=5,b=200:16e6,16e6,32p,8e6,16c6,8e6,8g6,8p,8g,8p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,16p,8c6,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e6,16g6,8a6,16f6,8g6,8e6,16c6,16d6,8b,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16c7,16p,16c7,16c7,p,16g6,16f#6,16f6,16d#6,16p,16e6,16p,16g#,16a,16c6,16p,16a,16c6,16d6,8p,16d#6,8p,16d6,8p,16c6";
//// Hàm customTone
//void customTone(int pin, int frequency, int duration) {
//  long halfPeriod = 1000000L / frequency / 2;  // Tính toán nửa chu kỳ của tần số, tính bằng micro giây
//  long cycles = frequency * duration / 1000;   // Tính toán số chu kỳ cần phát trong thời gian mong muốn
//
//  for (long i = 0; i < cycles; i++) {
//    digitalWrite(pin, HIGH);   // Đặt chân ở mức cao
//    delayMicroseconds(halfPeriod);  // Chờ nửa chu kỳ
//    digitalWrite(pin, LOW);    // Đặt chân ở mức thấp
//    delayMicroseconds(halfPeriod);  // Chờ nửa chu kỳ
//  }
//}
//
//// Hàm phát giai điệu từ chuỗi RTTTL
//void playRTTTL(const char *tune) {
//  int defaultDuration = 4;
//  int defaultOctave = 6;
//  int bpm = 200;
//
//  // Phần điều khiển
//  while (*tune != ':') tune++;  // Bỏ qua tên giai điệu
//  tune++;
//  while (*tune != ':') {
//    if (*tune == 'd') {
//      tune += 2; // Bỏ qua 'd='
//      defaultDuration = atoi(tune);
//    } else if (*tune == 'o') {
//      tune += 2; // Bỏ qua 'o='
//      defaultOctave = atoi(tune);
//    } else if (*tune == 'b') {
//      tune += 2; // Bỏ qua 'b='
//      bpm = atoi(tune);
//    }
//    while (*tune != ',' && *tune != ':') tune++;
//    if (*tune == ',') tune++;
//  }
//  tune++;
//
//  int beatDuration = 60000 / bpm;  // Thời gian của một nốt trọn vẹn (wholenote) tính bằng mili giây
//
//  // Phần nốt nhạc
//  while (*tune) {
//    int duration = defaultDuration;
//    int octave = defaultOctave;
//    char note = ' ';
//
//    // Đọc độ dài nốt nhạc
//    if (isdigit(*tune)) {
//      duration = 0;
//      while (isdigit(*tune)) {
//        duration = duration * 10 + (*tune - '0');
//        tune++;
//      }
//    }
//
//    // Đọc nốt nhạc
//    if (*tune == 'p') {
//      note = 'p';
//      tune++;
//    } else {
//      note = *tune;
//      tune++;
//      if (*tune == '#') {
//        note = toupper(note);
//        tune++;
//      }
//    }
//
//    // Đọc octave
//    if (isdigit(*tune)) {
//      octave = *tune - '0';
//      tune++;
//    }
//
//    // Đọc dấu chấm (staccato)
//    if (*tune == '.') {
//      duration = duration * 1.5;
//      tune++;
//    }
//
//    int time = beatDuration / duration;
//
//    if (note == 'p') {
//      delay(time);  // Nghỉ
//    } else {
//      int noteFrequency = noteToFrequency(note, octave);
//      customTone(buzzerPin, noteFrequency, time);
//    }
//
//    // Bỏ qua dấu phẩy
//    if (*tune == ',') tune++;
//  }
//}
//
//// Hàm để chuyển nốt nhạc thành tần số
//int noteToFrequency(char note, int octave) {
//  int baseFrequency;
//  switch (note) {
//    case 'c': baseFrequency = 261; break;
//    case 'C': baseFrequency = 277; break;
//    case 'd': baseFrequency = 294; break;
//    case 'D': baseFrequency = 311; break;
//    case 'e': baseFrequency = 329; break;
//    case 'f': baseFrequency = 349; break;
//    case 'F': baseFrequency = 370; break;
//    case 'g': baseFrequency = 392; break;
//    case 'G': baseFrequency = 415; break;
//    case 'a': baseFrequency = 440; break;
//    case 'A': baseFrequency = 466; break;
//    case 'b': baseFrequency = 494; break;
//    default: baseFrequency = 0; break;
//  }
//  return baseFrequency * pow(2, octave - 4);
//}
//
//void setup() {
//  pinMode(buzzerPin, OUTPUT);  // Cài đặt chân buzzerPin là đầu ra
//}
//
//void loop() {
//  const char *fireAlarm = "firealarm:d=4,o=6,b=200:c,c,c,p,c,c,c,p,c,c,c,p,c,c,c,p";
//  playRTTTL(fireAlarm);
//  delay(2000);  // Chờ 2 giây trước khi phát lại
//  playRTTTL(mario);
//  delay(2000);  // Chờ 2 giây trước khi phát lại
//}
#define BUZZER_PIN 4

// Hàm phát một nốt nhạc tùy chỉnh
void myTone(int pin, unsigned int frequency, unsigned long duration) {
  unsigned long period = 1000000L / frequency; // Tính chu kỳ của nốt nhạc
  unsigned long endTime = millis() + duration; // Tính thời gian kết thúc của nốt nhạc
  
  // Phát nốt nhạc cho đến khi hết thời gian duration
  while (millis() < endTime) {
    digitalWrite(pin, HIGH); // Đặt chân cảnh báo lên mức cao
    delayMicroseconds(period / 2); // Delay 1 nửa chu kỳ
    digitalWrite(pin, LOW); // Đặt chân cảnh báo xuống mức thấp
    delayMicroseconds(period / 2); // Delay 1 nửa chu kỳ
  }
}

void setup() {
  // Khởi tạo chân cảnh báo là OUTPUT
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Nghỉ 150 ms
  delay(150);

  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Nghỉ 150 ms
  delay(150);

  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Nghỉ 150 ms
  delay(150);

  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Phát nốt C6 trong 150 ms
  myTone(BUZZER_PIN, 1047, 150);

  // Nghỉ 150 ms
  delay(150);
}
