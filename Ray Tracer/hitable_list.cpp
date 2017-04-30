#include"hitable_list.h"

hitable_list::hitable_list(hitable ** pList, int pSize)
  :
  mList(pList),
  mSize(pSize)
{

}

bool hitable_list::hit(const ray & r, float tMin, float tMax, hit_record & hitRec) const
{

  bool hit_anything = false;
  float closest_so_far = tMax;
  hit_record tempRec;

  for (int i = 0; i < mSize; i += 1) {
    if (mList[i]->hit(r, tMin, closest_so_far, tempRec)) {
      hit_anything = true;
      closest_so_far = tempRec.t;
      hitRec = tempRec;
    }
  }

  return hit_anything;
}
