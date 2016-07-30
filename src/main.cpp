#include <iostream>
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "fifo.h"

using namespace std;
using namespace music_group;

/**
 * @brief This struct is the type that will be produced
 */
typedef struct {
  std::string food;
} packet_t;

/**
 * @brief Constant values
 */
const int  horses=10;
const int  monkeys=5;
const int  animals=horses+monkeys;
const string horse_food="apple";
const string monkey_food="banana";

/**
 * @brief This function represents the horse and pop function
 *
 * @param fifo   Pointer to the FIFO (Where they ask for food)
 * @param id     The number of the animal
 * @param feeded If they have eaten or not
 */
void horse(Fifo<packet_t>* fifo, int id, bool* feeded) {
  //cout << "Horse: " << id << endl;
  do {
    // The horse is patient and wait its turn
    packet_t my;
    fifo->pop(&my);
    if (my.food == horse_food) {
      cout << "Horse: " << id << " have eaten. " << endl;
      break;
    }
  } while (true);

  *feeded=true;

  return;
}

/**
 * @brief This function represents the monkey and pop_try function
 *
 * @param fifo   Pointer to the FIFO (Where they ask for food)
 * @param id     The number of the animal
 * @param feeded If they have eaten or not
 */
void monkey(Fifo<packet_t>* fifo, int id, bool* feeded) {
  error_t result=ERROR_OK;
  do {
    // The horse is patient and wait its turn
    packet_t my;
    result = fifo->pop(&my);
    if ((ERROR_OK == result) && (my.food == monkey_food)) {
      cout << "Desperate Monkey: " << id << " have eaten. " << endl;
      break;
    }
  } while (true);

  *feeded=true;

  return;
}

/**
 * @brief You journey starts HERE
 */
int main(int argc, char** argv) {
  Fifo<packet_t>* fifo_ = new Fifo<packet_t>(animals);
  string food;
  string qtd;
  int the_food=0;
  error_t result=ERROR_OK;

  thread threads[animals];
  bool have_eaten[animals];
  bool they_are_starving=true;

  for (int id = 0; id < animals; id++) {
    have_eaten[id]=false;
    if (id<horses) {
      threads[id] = thread(horse, fifo_, id, &have_eaten[id]);
    } else {
      threads[id] = thread(monkey, fifo_, id-horses, &have_eaten[id]);
    }
  }

  cout << "################################################################" << endl;
  cout << "# The purpose of this game is to feed some animals. The horses #" << endl;
  cout << "# are patient and can wait their turn (pop function), but the  #" << endl;
  cout << "# monkeys are desperate (pop_try) and they can't wait.         #" << endl;
  cout << "# You are the feeder (push) and must help them with food,      #" << endl;
  cout << "# but remember that horses eat apples and monkeys bananas.     #" << endl;
  cout << "#                                                              #" << endl;
  cout << "# Hit: start with the patient animals :-)                      #" << endl;
  cout << "################################################################" << endl;

  cout << endl;

  do {
    cout << "you must fill the packet (ex. 3 apple): ";
    cin >> qtd >> food;

    if ((food != horse_food) && (food != monkey_food)) {
      cout << "Sorry, they only eat banana or apple" << endl;
    }
    the_food = stoi(qtd);

    for (int i=0; i<the_food; i++) {
      packet_t packet;
      packet.food=food;

      result=fifo_->push(packet);
      if (ERROR_OK != result) {
        cout << "TOO MUCH FOOD " << endl;
        break;
      }
    }
    cout << "Wait... they are checking the packet " << endl;
    sleep(3);

    // Check if all animals are fed
    they_are_starving=false;
    for (int j=0; j<animals; j++) {
      if (!have_eaten[j]) {
        they_are_starving=true;
        cout << "DON'T STOP, SOME ANIMALS ARE STARVING" << endl;
        break;
      }
    }

    // Clean the packet
    for (int i=0; i<the_food; i++) {
      packet_t packet;
      packet.food=food;

      result=fifo_->pop_try(&packet);
      if (ERROR_OK != result) {
        break;
      }
    }

  } while (they_are_starving);

  for (int i = 0; i < animals; ++i) {
    threads[i].join();
  }

  cout << "NOW THEY ARE HAPPY, THANK YOU                 " << endl;
  cout << "                               w  c(..)o   (  " << endl;
  cout << "                    ;;          \\__(-)    __)" << endl;
  cout << "                  ,;;'\\            /\\   (   " << endl;
  cout << "       __       ,;;' ' \\          /(_)___)   " << endl;
  cout << "  ,;(      )    /  |              w /|        " << endl;
  cout << " ,;' \\    /-.,,(   )                | \\     " << endl;
  cout << "      ) /       ) /                 m  m      " << endl;
  cout << "      ||        ||                            " << endl;
  cout << "      (_\\       (_\\                         " << endl;

  delete fifo_;

  return 0;
}
