import React from "react";
import GameCard from "./GameCard";
import { Box } from "@chakra-ui/react";
import { getRandomInt } from "../utils/Common";

interface UserDeckProp {
    cards: GameCard[]
    debugBorder?: boolean
}

interface UserDeckState {
    cards: GameCard[]
    debugBorder?: boolean
}

export default class UserDeck extends React.Component<UserDeckProp, UserDeckState> {
    constructor(props: UserDeckProp) {
        super(props);

        this.state = {
            cards: props.cards,
            debugBorder: props.debugBorder
        };
    }

    render() {
        return (
            <Box
                // sx={{ border: { '10px solid white' : '' } }}
                justifyContent='center'
                display='flex'
                flexDirection='row'
                gap='10px'
                marginBottom='100px'>

                {this.state.cards.map((card: GameCard) => (
                    <GameCard
                        isActive={card.state.isActive}
                        value={card.state.value}
                    />
                ))}
            </Box>
        )
    }

}
