/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsLTE library.
 *
 * srsLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "srslte/sync/sfo.h"

/* Estimate SFO based on the array of time estimates t0
 * of length len. The parameter period is the time between t0 samples
 */
float srslte_sfo_estimate(int *t0, int len, float period) {
  int i;
  float sfo=0.0;
  for (i=1;i<len;i++) {
    sfo += (t0[i]-t0[i-1])/period/len;
  }
  return sfo;
}

/* Same as srslte_sfo_estimate but period is non-uniform.
 * Vector t is the sampling time times period for each t0
 */
float srslte_sfo_estimate_period(int *t0, int *t, int len, float period) {
  int i;
  float sfo=0.0;
  for (i=1;i<len;i++) {
    if (abs(t0[i]-t0[i-1]) < 5000) {
      sfo += (t0[i]-t0[i-1])/(t[i] - t[i-1])/period;
    }
  }
  return sfo/(len-2);
}
