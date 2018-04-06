#include "FoodInfluence.h"

FoodInfluence::FoodInfluence(const V2d &pos)
        :pos_(pos)
{

}

float FoodInfluence::probe(const V2d &v) const {
    return -1.f/((v-pos_).getNormSq() + 1.f);
}
