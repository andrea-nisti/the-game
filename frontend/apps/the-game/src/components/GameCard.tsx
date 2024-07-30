import React from "react";
import { getRandomInt } from "../utils/Common";
import { Card, CardBody, Text } from "@chakra-ui/react";

interface GameCardProp {
    value: number;
    isActive: boolean;
}

interface GameCardState {
    value: number;
    isActive: boolean;
}

class GameCard extends React.Component<GameCardProp, GameCardState> {
    constructor(props: GameCardProp) {
        super(props);

        this.state = {
            value: props.value,
            isActive: false
        };
    }

    handleCardClick = () => {
        this.setState({ isActive: !this.state.isActive });
      };

    render() {
        return (
            <Card onClick={this.handleCardClick} border={this.state.isActive ? '2px solid rgb(112, 185, 212)' : 'inherit'} variant='filled' align='center' width='100px' height='170px' >
                <CardBody fontSize='30px' alignItems='stretch' alignContent='center'>
                    <Text align='center'> {this.state.value} </Text>
                </CardBody>
            </Card>
        )
    }
}

export default GameCard