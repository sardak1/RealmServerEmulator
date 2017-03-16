-- Utility functions for dealing with tables

TableUtil = {
    remove = function(array, item)
        for index, element in ipairs(array) do
            if item == element then
                table.remove(array, index)
                return true
            end
        end

        return false
    end,
}
