import React from 'react';
import ReactDOM from 'react-dom';
import App from './components/App.jsx';
import {addDivToBody, addLinkToHead} from './modules/dom';

require('./styles/index.css');

Promise.all([
  addLinkToHead('https://cdnjs.cloudflare.com/ajax/libs/normalize/7.0.0/normalize.css'),
  addLinkToHead('https://fonts.googleapis.com/css?family=EB+Garamond')
]).then(() => {
  ReactDOM.render(<App />, addDivToBody({'class': 'root'}));
}).catch(error => {
  console.error('Failed to load styles.', error);
});
