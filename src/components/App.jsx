import React, {Component} from 'react';
import {BrowserRouter as Router, Route} from 'react-router-dom'

import Intro from './Intro';
import Outro from './Outro';
import Year from './Year';

export default class App extends Component {
  render() {
    return (
      <Router>
        <div className="root">
          <Route exact path="/" component={Intro} />
          <Route exact path="/end" component={Outro} />
          <Route path="/year/:year" component={Year} />
        </div>
      </Router>
    );
  }
}
