import React from "react";
import { getRandomInt } from "../utils/Common";
import { Card, CardBody, Text } from "@chakra-ui/react";

interface GameCardProp {
    value: number;
    index: number;
    isActive?: boolean;
    onClick?: () => void
}

interface GameCardState {
    value: number;
    index: number;
}

class GameCard extends React.Component<GameCardProp, GameCardState> {

    constructor(props: GameCardProp) {
        super(props);

        this.state = {
            value: props.value,
            index: props.index,
        };
    }

    render() {
        return (
            <Card onClick={this.props.onClick} border={this.props.isActive ? '2px solid rgb(112, 185, 212)' : 'inherit'} variant='filled' align='center' width='100px' height='170px' >
                <CardBody fontSize='30px' alignItems='stretch' alignContent='center'>
                    <Text align='center'> {this.state.value} </Text>
                </CardBody>
            </Card>
        )
    }
}

export default GameCard