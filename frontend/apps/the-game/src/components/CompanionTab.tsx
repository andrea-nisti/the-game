import React from "react";
import Player from "./Player";
import { Box } from "@chakra-ui/react";

interface CompanionTabProp {
    players: Player[]
}

interface CompanionTabState {
    players: Player[]
}

export default class CompanionTab extends React.Component<CompanionTabProp, CompanionTabState> {
    constructor(props: CompanionTabProp) {
        super(props);

        this.state = {
            players: props.players,
        };
    }

    render() {
        return (
            <Box
                // sx={{ border: '10px solid white' }}
                justifyContent='center'
                alignItems='flex-end'
                display='flex'
                flexDirection='row'
                gap='50px'>
                {this.state.players.map((player: Player) => (
                    <Player
                        name={player.state.name}
                        cardNumber={player.state.cardNumber}
                        isUser={false}
                    />

                ))}
            </Box>
        )
    }
}