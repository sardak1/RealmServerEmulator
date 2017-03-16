Players = Players or {}


function add_player_to_game(player)
    table.insert(Players, player)

    Server.send_message(player, "ack-connect")
end


function remove_player_from_game(player)
    TableUtil.remove(Players, player)
end
