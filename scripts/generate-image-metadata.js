const R = require("ramda");
const sizeOf = require("image-size");
const fs = require("fs");
const path = require("path");

const years = fs.readdirSync(path.join(__dirname, "..", "src", "assets"));

const getImagesPath = year =>
  path.join(__dirname, "..", "src", "assets", year, "med");

const getImagePathsForYear = year =>
  R.map(
    fileName => path.join(getImagesPath(year), fileName),
    fs.readdirSync(getImagesPath(year))
  );

const COUNT_BY_YEAR = R.reduce(
  (acc, year) => ({ ...acc, [year]: getImagePathsForYear(year).length }),
  {},
  years
);

const getDimensions = path =>
  R.pipe(({ height, width }) => ({ height, width }))(sizeOf(path));

const imagePathToId = R.pipe(
  R.split("/"),
  R.last,
  R.split("-"),
  R.last,
  R.split("."),
  R.head
);
const getDimensionsById = year =>
  R.pipe(
    R.map(p => [p, imagePathToId(p)]),
    R.reduce((acc, [p, id]) => ({ ...acc, [id]: getDimensions(p) }), {})
  )(getImagePathsForYear(year));

const DIMENSIONS_BY_ID_BY_YEAR = R.reduce(
  (acc, year) => ({ ...acc, [year]: { ...getDimensionsById(year) } }),
  {},
  years
);

fs.writeFileSync(
  path.join(__dirname, "..", "src", "constants", "image-metadata.json"),
  JSON.stringify(DIMENSIONS_BY_ID_BY_YEAR, null, 2)
);
