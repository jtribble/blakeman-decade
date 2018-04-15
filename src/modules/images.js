import * as R from 'ramda';
import constants from '../constants';
const {COUNT_BY_YEAR} = constants;

export const getImagePaths = size => year => R.pipe(
  R.times(R.add(1)),
  R.map(i => `/assets/${year}/${size}/${year}-${i}.jpg`)
)(COUNT_BY_YEAR[year]);

export const getValidYears = () => R.pipe(
  R.filter(R.lt(0)),
  R.keys,
)(COUNT_BY_YEAR);

export const getSmallImagePaths = getImagePaths('sm');
export const getMediumImagePaths = getImagePaths('med');
