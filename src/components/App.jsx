import React, {Component} from 'react';
import {BrowserRouter as Router, Route} from 'react-router-dom'

import Intro from './Intro';
import Years from './Years';

export default class App extends Component {
  render() {
    return (
      <Router>
        <div className="root">
          <Route exact path="/" component={Intro} />
          <Route path="/years" component={Years} />
        </div>
      </Router>
    );
  }
}
