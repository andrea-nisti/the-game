import { Avatar, AvatarBadge, Box, Card, CardBody, Text } from "@chakra-ui/react";
import React from "react";
import { getRandomInt } from "../utils/Common";

interface PlayerProp {
    name: string,
    isUser: boolean,
    cardNumber: number
    imgSource?: Promise<string>
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
            <Box key= {this.state.name} display='flex' flexDirection='row' justifyContent='center' alignItems='center' gap='1vh'>
                <Avatar
                    size='xl'
                >
                    <AvatarBadge boxSize='1.25em' bg='green.500' />
                </Avatar>

                <Card width='6vh' height='9vh'>
                    <CardBody justifyContent='center' alignItems='center' justifySelf='center' >
                        <Text align='center' alignContent='center' fontSize='3vh'>{this.state.cardNumber} </Text>
                    </CardBody>
                </Card>
            </Box>
        )
    }
}
