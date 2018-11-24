import React, { Component } from 'react';
import { Link } from 'react-router-dom';
import { Jumbotron, Grid, Row, Col, Image, Button } from 'react-bootstrap';
import './Home.css';

class Home extends Component {
    render() {
        return (
            <Grid>
                <Jumbotron>
                    <h2>Please scan RFID card to continue.</h2>
                    <p>For all troubleshooting needs, please contact Tony Stark at 1-800-435-4648.</p>
                </Jumbotron>
            </Grid>
        );
    }
}

export default Home;