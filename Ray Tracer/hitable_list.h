#ifndef INC_HITABLE_LIST_H
#define INC_HITABLE_LIST_H

#include"hitable.h"

class hitable_list : public hitable {

private:

  hitable **mList;
  int mSize;

public:

  hitable_list(hitable **pList, int pSize);
  virtual bool hit(const ray & r, float tMin, float tMax, hit_record & hitRec) const override;

};

#endif
