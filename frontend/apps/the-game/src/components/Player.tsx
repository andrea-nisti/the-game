import { Avatar, Box, Card, CardBody, Text } from "@chakra-ui/react";
import React from "react";
import { getRandomInt } from "../utils/Common";

interface PlayerProp {
    name: string,
    isUser: boolean,
    cardNumber: number
}

interface PlayerState {
    name: string,
    isUser: boolean,
    cardNumber: number
}

export default class Player extends React.Component<PlayerProp, PlayerState> {
    constructor(props: PlayerProp) {
        super(props);

        this.state = {
            name: props.name,
            isUser: false,
            cardNumber: props.cardNumber,
        };
    }

    render() {
        return (
            <Box display='flex' flexDirection='row' justifyContent='center' alignItems='center' gap='10px'>
                <Avatar name={this.state.name} />

                <Card width='50px' height='75px'>
                    <CardBody justifyContent='center' alignItems='center' justifySelf='center'>
                        <Text >{this.state.cardNumber} </Text>
                    </CardBody>
                </Card>
            </Box>
        )
    }
}
